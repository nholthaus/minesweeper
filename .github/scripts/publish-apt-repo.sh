#!/usr/bin/env bash

set -euo pipefail

if [[ $# -ne 2 ]]; then
	echo "usage: $0 <site-root> <deb-dir>" >&2
	exit 1
fi

if [[ -z "${APT_GPG_KEY_ID:-}" ]]; then
	echo "APT_GPG_KEY_ID is required" >&2
	exit 1
fi

SITE_ROOT=$1
DEB_DIR=$2
APT_ROOT="${SITE_ROOT}/apt"
CODENAME=stable
COMPONENT=main
ARCH=amd64
POOL_DIR="${APT_ROOT}/pool/${COMPONENT}/m/minesweeper"
DIST_DIR="${APT_ROOT}/dists/${CODENAME}"
BINARY_DIR="${DIST_DIR}/${COMPONENT}/binary-${ARCH}"
APT_CONF="${APT_ROOT}/apt-ftparchive.conf"
PASSPHRASE_ARGS=()

mkdir -p "${POOL_DIR}" "${BINARY_DIR}"

find "${DEB_DIR}" -maxdepth 1 -type f -name '*.deb' -exec cp -f {} "${POOL_DIR}/" \;

dpkg-scanpackages --multiversion "${APT_ROOT}/pool" > "${BINARY_DIR}/Packages"
gzip -9 -c "${BINARY_DIR}/Packages" > "${BINARY_DIR}/Packages.gz"

cat > "${APT_CONF}" <<EOF
APT::FTPArchive::Release {
  Origin "Minesweeper";
  Label "Minesweeper";
  Suite "${CODENAME}";
  Codename "${CODENAME}";
  Architectures "${ARCH}";
  Components "${COMPONENT}";
  Description "Minesweeper APT repository";
};
EOF

apt-ftparchive -c "${APT_CONF}" release "${DIST_DIR}" > "${DIST_DIR}/Release"
rm -f "${APT_CONF}"

if [[ -n "${APT_GPG_PASSPHRASE:-}" ]]; then
	PASSPHRASE_ARGS=(--passphrase "${APT_GPG_PASSPHRASE}")
fi

gpg --batch --yes --pinentry-mode loopback \
	--local-user "${APT_GPG_KEY_ID}" \
	"${PASSPHRASE_ARGS[@]}" \
	--clearsign \
	--output "${DIST_DIR}/InRelease" \
	"${DIST_DIR}/Release"

gpg --batch --yes --pinentry-mode loopback \
	--local-user "${APT_GPG_KEY_ID}" \
	"${PASSPHRASE_ARGS[@]}" \
	--armor \
	--detach-sign \
	--output "${DIST_DIR}/Release.gpg" \
	"${DIST_DIR}/Release"

gpg --batch --yes --armor --export "${APT_GPG_KEY_ID}" > "${APT_ROOT}/minesweeper-archive-keyring.asc"

cat > "${SITE_ROOT}/index.html" <<EOF
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Minesweeper APT Repository</title>
</head>
<body>
  <h1>Minesweeper APT Repository</h1>
  <p>APT base URL: <code>https://nholthaus.github.io/minesweeper/apt</code></p>
  <p>Distribution: <code>${CODENAME}</code></p>
  <p>Component: <code>${COMPONENT}</code></p>
  <p>Public key: <a href="apt/minesweeper-archive-keyring.asc">minesweeper-archive-keyring.asc</a></p>
</body>
</html>
EOF

touch "${SITE_ROOT}/.nojekyll"
