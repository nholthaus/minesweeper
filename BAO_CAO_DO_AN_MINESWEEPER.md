# BÁO CÁO ĐỒ ÁN THỰC HÀNH

## MỞ RỘNG ỨNG DỤNG MINESWEEPER VỚI QT FRAMEWORK

---

**Môn học:** Lập trình hướng đối tượng / Phát triển phần mềm  
**Sinh viên thực hiện:** [Họ và tên sinh viên]  
**Mã sinh viên:** [MSSV]  
**Lớp:** [Tên lớp]  
**Giảng viên hướng dẫn:** [Tên giảng viên]  
**Năm học:** 2024-2025

---

## MỤC LỤC

1. [Giới thiệu đề tài](#1-giới-thiệu-đề-tài)
2. [Phân tích hệ thống ban đầu (Legacy System)](#2-phân-tích-hệ-thống-ban-đầu-legacy-system)
3. [Phần mở rộng và phát triển thêm](#3-phần-mở-rộng-và-phát-triển-thêm)
4. [Kiến trúc và thiết kế](#4-kiến-trúc-và-thiết-kế)
5. [Chi tiết triển khai](#5-chi-tiết-triển-khai)
6. [Kết quả và đánh giá](#6-kết-quả-và-đánh-giá)
7. [Kết luận](#7-kết-luận)
8. [Tài liệu tham khảo](#8-tài-liệu-tham-khảo)

---

## 1. Giới thiệu đề tài

### 1.1. Mô tả dự án

Đồ án này thực hiện việc **MỞ RỘNG** một ứng dụng Minesweeper có sẵn (legacy system) được phát triển bởi nhà phát triển nholthaus trên GitHub. Mục tiêu là bổ sung các tính năng mới mà không làm thay đổi phần lõi (core gameplay) của trò chơi gốc.

### 1.2. Mục tiêu

| #   | Mục tiêu                  | Mô tả                                                |
| --- | ------------------------- | ---------------------------------------------------- |
| 1   | **Bảo toàn hệ thống gốc** | Giữ nguyên toàn bộ core gameplay như một "black box" |
| 2   | **Mở rộng tính năng**     | Thêm các tính năng mới thông qua lớp extension       |
| 3   | **Tách biệt kiến trúc**   | Thiết kế sao cho code mở rộng tách biệt với code gốc |
| 4   | **Tương thích ngược**     | Đảm bảo các tính năng gốc vẫn hoạt động bình thường  |

### 1.3. Công nghệ sử dụng

- **Framework:** Qt 6.7.2
- **Compiler:** MinGW 11.2.0 (64-bit)
- **Ngôn ngữ:** C++20
- **Build System:** CMake 3.16+
- **IDE:** Visual Studio Code

---

## 2. Phân tích hệ thống ban đầu (Legacy System)

### 2.1. Tổng quan hệ thống gốc

Hệ thống Minesweeper gốc được phát triển bởi nhà phát triển **nholthaus** với các thành phần chính sau:

```
┌─────────────────────────────────────────────────────────────┐
│                    LEGACY SYSTEM                            │
│                  (Hệ thống ban đầu)                         │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│   ┌─────────────┐    ┌─────────────┐    ┌─────────────┐    │
│   │ MainWindow  │    │  GameBoard  │    │    Tile     │    │
│   │   (Gốc)     │◄───┤   (Gốc)     │◄───┤   (Gốc)     │    │
│   └─────────────┘    └─────────────┘    └─────────────┘    │
│          │                  │                  │            │
│          │                  │                  │            │
│   ┌─────────────┐    ┌─────────────┐    ┌─────────────┐    │
│   │  MineTimer  │    │MineCounter  │    │  HighScore  │    │
│   │   (Gốc)     │    │   (Gốc)     │    │   (Gốc)     │    │
│   └─────────────┘    └─────────────┘    └─────────────┘    │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### 2.2. Các file thuộc hệ thống gốc (Legacy Files)

| File                    | Vai trò                 | Trạng thái                      |
| ----------------------- | ----------------------- | ------------------------------- |
| `mainwindow.h/cpp`      | Giao diện chính gốc     | **Không sửa đổi**               |
| `gameboard.h/cpp`       | Quản lý bảng game       | **Sửa đổi nhỏ** (thêm accessor) |
| `tile.h/cpp`            | Ô vuông riêng lẻ        | **Sửa đổi nhỏ** (font-size)     |
| `mineCounter.h/cpp`     | Hiển thị số mìn còn lại | **Không sửa đổi**               |
| `minetimer.h/cpp`       | Đếm thời gian           | **Không sửa đổi**               |
| `highScore.h/cpp`       | Lưu điểm cao            | **Không sửa đổi**               |
| `highScoreModel.h/cpp`  | Model điểm cao          | **Không sửa đổi**               |
| `highScoreDialog.h/cpp` | Dialog điểm cao         | **Không sửa đổi**               |
| `gameStats.h/cpp`       | Thống kê game           | **Không sửa đổi**               |
| `gameStatsDialog.h/cpp` | Dialog thống kê         | **Không sửa đổi**               |
| `versionChecker.h/cpp`  | Kiểm tra phiên bản      | **Không sửa đổi**               |

### 2.3. Phân tích class MainWindow gốc

```cpp
// mainwindow.h - Class gốc
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

signals:
    void startGame();
    void victory();
    void defeat();
    void startNewGame();

private:
    // Core components (Thành phần lõi)
    GameBoard*    gameBoard;      // Bảng game
    MineCounter*  mineCounter;    // Đếm mìn
    MineTimer*    mineTimer;      // Đếm giờ
    QPushButton*  newGame;        // Nút game mới
    QTimer*       gameClock;      // Đồng hồ game

    // State Machine (Máy trạng thái đơn giản)
    QStateMachine* m_machine;
    QState* unstartedState;       // Chưa bắt đầu
    QState* inProgressState;      // Đang chơi
    QState* victoryState;         // Thắng
    QState* defeatState;          // Thua

    // Menu (Menu gốc)
    QMenu* gameMenu;              // Menu Game
    QMenu* difficultyMenu;        // Menu Difficulty
    QMenu* helpMenu;              // Menu Help

    // Difficulty settings
    HighScore::Difficulty difficulty;
    quint32 numRows, numCols, numMines;
};
```

### 2.4. Tính năng của hệ thống gốc

#### 2.4.1. Các tính năng có sẵn

| Tính năng         | Mô tả                 | Vị trí                    |
| ----------------- | --------------------- | ------------------------- |
| **New Game**      | Bắt đầu game mới      | Menu Game → New Game (F2) |
| **Difficulty**    | Chọn độ khó           | Menu Game → Difficulty    |
| **Beginner**      | 9x9, 10 mìn           | Menu Difficulty           |
| **Intermediate**  | 16x16, 40 mìn         | Menu Difficulty           |
| **Expert**        | 16x30, 99 mìn         | Menu Difficulty           |
| **High Scores**   | Xem điểm cao          | Menu Game → High Scores   |
| **Statistics**    | Xem thống kê          | Menu Game → Statistics    |
| **Version Check** | Kiểm tra bản cập nhật | Menu Help                 |

#### 2.4.2. Gameplay gốc (Core Gameplay)

```
┌─────────────────────────────────────────────────────────┐
│                    CORE GAMEPLAY                        │
│              (Được giữ nguyên như Black Box)            │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  1. Click trái  → Mở ô                                  │
│  2. Click phải  → Đặt/bỏ cờ                             │
│  3. Mở ô mìn    → Thua                                  │
│  4. Mở hết ô    → Thắng                                 │
│  5. Số trên ô   → Số mìn xung quanh                     │
│  6. Ô trống     → Tự động mở các ô lân cận              │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

### 2.5. State Machine gốc

```
                    ┌─────────────┐
                    │  Unstarted  │◄──────────────────┐
                    │   State     │                   │
                    └──────┬──────┘                   │
                           │ startGame()             │
                           ▼                         │
                    ┌─────────────┐            startNewGame()
                    │ InProgress  │                   │
                    │   State     │───────────────────┤
                    └──────┬──────┘                   │
                           │                         │
              ┌────────────┼────────────┐            │
              │            │            │            │
         victory()         │       defeat()          │
              │            │            │            │
              ▼            │            ▼            │
       ┌──────────┐        │     ┌──────────┐        │
       │ Victory  │        │     │  Defeat  │        │
       │  State   │────────┴─────┤  State   │────────┘
       └──────────┘              └──────────┘
```

---

## 3. Phần mở rộng và phát triển thêm

### 3.1. Tổng quan các tính năng mở rộng

Sinh viên đã phát triển thêm các tính năng sau, được triển khai trong các file hoàn toàn mới:

```
┌─────────────────────────────────────────────────────────────┐
│                   EXTENSION LAYER                           │
│              (Lớp mở rộng - Do sinh viên phát triển)        │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│   ┌─────────────────┐    ┌─────────────────┐               │
│   │ MainWindowExt   │◄───┤ GameController  │               │
│   │   (Mới)         │    │    (Mới)        │               │
│   └────────┬────────┘    └────────┬────────┘               │
│            │                      │                         │
│   ┌────────┴────────┬─────────────┼─────────────┐          │
│   │                 │             │             │          │
│   ▼                 ▼             ▼             ▼          │
│ ┌───────────┐ ┌───────────┐ ┌───────────┐ ┌───────────┐   │
│ │ThemeManager│ │HintSystem │ │PauseOverlay│ │Leaderboard│   │
│ │  (Mới)    │ │  (Mới)    │ │   (Mới)   │ │  (Mới)    │   │
│ └───────────┘ └───────────┘ └───────────┘ └───────────┘   │
│                                                             │
│   ┌───────────────┐    ┌───────────────┐                   │
│   │StartMenuDialog│    │CustomGameDialog│                   │
│   │    (Mới)      │    │    (Mới)      │                   │
│   └───────────────┘    └───────────────┘                   │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### 3.2. Danh sách file mở rộng (Extension Files)

| File                      | Vai trò                    | Dòng code      |
| ------------------------- | -------------------------- | -------------- |
| `gameEnums.h`             | Định nghĩa các enum mới    | ~50            |
| `gameController.h/cpp`    | Quản lý trạng thái và mode | ~350           |
| `themeManager.h/cpp`      | Quản lý theme Light/Dark   | ~250           |
| `startMenuDialog.h/cpp`   | Dialog Start Menu mới      | ~450           |
| `customGameDialog.h/cpp`  | Dialog tùy chỉnh game      | ~200           |
| `pauseOverlay.h/cpp`      | Overlay khi tạm dừng       | ~200           |
| `hintSystem.h/cpp`        | Hệ thống gợi ý             | ~150           |
| `leaderboard.h/cpp`       | Bảng xếp hạng mở rộng      | ~200           |
| `leaderboardDialog.h/cpp` | Dialog bảng xếp hạng       | ~250           |
| `mainwindow_ext.h/cpp`    | MainWindow mở rộng         | ~1200          |
| **Tổng cộng**             |                            | **~3300 dòng** |

### 3.3. Chi tiết các tính năng mở rộng

#### 3.3.1. Start Menu Dialog

**Mục đích:** Thay thế việc chọn tùy chọn từ menu bar bằng giao diện Start Menu hiện đại.

**Luồng hoạt động:**

```
┌─────────────────────────────────────────────────────────────┐
│                    START MENU DIALOG                        │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│   ┌─────────────────────────────────────────────────────┐  │
│   │                   MAIN PAGE                          │  │
│   │                                                      │  │
│   │    ┌────────────────────────────────────┐           │  │
│   │    │         🎮 NEW GAME                │           │  │
│   │    └────────────────────────────────────┘           │  │
│   │    ┌────────────────────────────────────┐           │  │
│   │    │         🏆 LEADERBOARD             │           │  │
│   │    └────────────────────────────────────┘           │  │
│   │    ┌────────────────────────────────────┐           │  │
│   │    │         📊 STATISTICS              │           │  │
│   │    └────────────────────────────────────┘           │  │
│   │    ┌────────────────────────────────────┐           │  │
│   │    │         ⚙️  SETTINGS               │           │  │
│   │    └────────────────────────────────────┘           │  │
│   │    ┌────────────────────────────────────┐           │  │
│   │    │         🚪 EXIT                    │           │  │
│   │    └────────────────────────────────────┘           │  │
│   │                                                      │  │
│   └─────────────────────────────────────────────────────┘  │
│                           │                                 │
│                           ▼ Click "New Game"               │
│   ┌─────────────────────────────────────────────────────┐  │
│   │                 MODE SELECTION PAGE                  │  │
│   │                                                      │  │
│   │    ┌────────────────────────────────────┐           │  │
│   │    │         🎯 NORMAL MODE             │           │  │
│   │    │      (Chơi bình thường)            │           │  │
│   │    └────────────────────────────────────┘           │  │
│   │    ┌────────────────────────────────────┐           │  │
│   │    │         ⏱️  TIME ATTACK MODE       │           │  │
│   │    │      (Chế độ đếm ngược)            │           │  │
│   │    └────────────────────────────────────┘           │  │
│   │    ┌────────────────────────────────────┐           │  │
│   │    │         ← BACK                     │           │  │
│   │    └────────────────────────────────────┘           │  │
│   │                                                      │  │
│   └─────────────────────────────────────────────────────┘  │
│                           │                                 │
│                           ▼ Chọn mode                      │
│   ┌─────────────────────────────────────────────────────┐  │
│   │                DIFFICULTY SELECTION PAGE             │  │
│   │                                                      │  │
│   │    ┌────────────────────────────────────┐           │  │
│   │    │         😊 BEGINNER (9x9, 10 mìn) │           │  │
│   │    └────────────────────────────────────┘           │  │
│   │    ┌────────────────────────────────────┐           │  │
│   │    │         😐 INTERMEDIATE (16x16)   │           │  │
│   │    └────────────────────────────────────┘           │  │
│   │    ┌────────────────────────────────────┐           │  │
│   │    │         😎 EXPERT (16x30, 99 mìn) │           │  │
│   │    └────────────────────────────────────┘           │  │
│   │    ┌────────────────────────────────────┐           │  │
│   │    │         🔧 CUSTOM                  │           │  │
│   │    └────────────────────────────────────┘           │  │
│   │    ┌────────────────────────────────────┐           │  │
│   │    │         ← BACK                     │           │  │
│   │    └────────────────────────────────────┘           │  │
│   │                                                      │  │
│   └─────────────────────────────────────────────────────┘  │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

#### 3.3.2. Game Modes (Chế độ chơi)

| Chế độ               | Mô tả                      | Điểm khác biệt                    |
| -------------------- | -------------------------- | --------------------------------- |
| **Normal Mode**      | Chế độ chơi truyền thống   | Thời gian đếm lên, không giới hạn |
| **Time Attack Mode** | Chế độ đếm ngược thời gian | Phải hoàn thành trước khi hết giờ |

**Thời gian Time Attack theo độ khó:**

```cpp
int getTimeAttackSeconds(Difficulty difficulty) {
    switch (difficulty) {
        case Beginner:      return 120;  // 2 phút
        case Intermediate:  return 300;  // 5 phút
        case Expert:        return 600;  // 10 phút
        case Custom:        return 300;  // 5 phút (mặc định)
    }
}
```

#### 3.3.3. Theme System (Hệ thống giao diện)

```cpp
// themeManager.h
class ThemeManager : public QObject {
    Q_OBJECT

public:
    static ThemeManager* instance();  // Singleton pattern

    void setTheme(GameEnums::Theme theme);
    GameEnums::Theme currentTheme() const;

    QString getStyleSheet(GameEnums::Theme theme);

signals:
    void themeChanged(GameEnums::Theme theme);

private:
    GameEnums::Theme m_currentTheme = GameEnums::Theme::Light;
};
```

**So sánh Light/Dark Theme:**

| Thành phần | Light Theme | Dark Theme |
| ---------- | ----------- | ---------- |
| Background | #f0f0f0     | #2d2d2d    |
| Text       | #000000     | #ffffff    |
| Button     | #ffffff     | #3d3d3d    |
| Border     | #c0c0c0     | #555555    |
| Accent     | #0078d4     | #4cc2ff    |

#### 3.3.4. Pause System (Hệ thống tạm dừng)

```
┌─────────────────────────────────────────────────────────────┐
│                     PAUSE OVERLAY                           │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│         ╔═══════════════════════════════════════╗          │
│         ║                                       ║          │
│         ║              ⏸ PAUSED                ║          │
│         ║                                       ║          │
│         ║    ┌─────────────────────────┐       ║          │
│         ║    │        RESUME           │       ║          │
│         ║    └─────────────────────────┘       ║          │
│         ║    ┌─────────────────────────┐       ║          │
│         ║    │       NEW GAME          │       ║          │
│         ║    └─────────────────────────┘       ║          │
│         ║                                       ║          │
│         ╚═══════════════════════════════════════╝          │
│                                                             │
│  (Game board bị ẩn để tránh gian lận)                      │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

#### 3.3.5. Hint System (Hệ thống gợi ý)

```cpp
// hintSystem.h
class HintSystem : public QObject {
    Q_OBJECT

public:
    void setGameBoard(GameBoard* board);
    void resetHints();
    int hintsRemaining() const;

public slots:
    void provideHint();

signals:
    void hintsRemainingChanged(int remaining);
    void hintProvided(int row, int col);

private:
    GameBoard* m_gameBoard = nullptr;
    int m_hintsRemaining = 3;  // Mặc định 3 gợi ý

    Tile* findSafeTile();  // Tìm ô an toàn
};
```

**Logic tìm ô an toàn:**

```
1. Duyệt tất cả các ô trong board
2. Lọc các ô chưa mở và không có mìn
3. Ưu tiên ô cạnh số (có thông tin nhiều hơn)
4. Chọn ngẫu nhiên nếu có nhiều ô phù hợp
5. Highlight ô được chọn trong 2 giây
```

#### 3.3.6. Custom Game Dialog

**Input validation:**

```cpp
// Giới hạn
const int MIN_ROWS = 5;
const int MAX_ROWS = 30;
const int MIN_COLS = 5;
const int MAX_COLS = 50;
const int MIN_MINES = 1;
// MAX_MINES = (rows * cols) - 1

// Validation
bool isValid = (rows >= MIN_ROWS && rows <= MAX_ROWS) &&
               (cols >= MIN_COLS && cols <= MAX_COLS) &&
               (mines >= MIN_MINES && mines < rows * cols);
```

### 3.4. So sánh trước và sau mở rộng

| Tính năng         | Legacy (Trước)               | Extension (Sau)            |
| ----------------- | ---------------------------- | -------------------------- |
| **Chế độ chơi**   | Chỉ có Normal                | Normal + Time Attack       |
| **Độ khó**        | Beginner/Intermediate/Expert | + Custom                   |
| **Giao diện**     | Menu bar truyền thống        | Start Menu Dialog          |
| **Theme**         | Theo hệ thống                | Light / Dark (chọn được)   |
| **Tạm dừng**      | Không có                     | Pause với overlay          |
| **Gợi ý**         | Không có                     | 3 hints mỗi game           |
| **State Machine** | 4 states cơ bản              | Mở rộng với Paused state   |
| **Bảng xếp hạng** | Theo difficulty              | Lọc theo mode + difficulty |

---

## 4. Kiến trúc và thiết kế

### 4.1. Mô hình kiến trúc Extension Layer

```
┌─────────────────────────────────────────────────────────────────────┐
│                        APPLICATION LAYER                            │
│                         (main.cpp)                                  │
└───────────────────────────────┬─────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────────────┐
│                       EXTENSION LAYER                               │
│                      (MainWindowExt)                                │
│  ┌─────────────────────────────────────────────────────────────┐   │
│  │ MainWindowExt : QMainWindow                                  │   │
│  │  - GameController*  m_controller                             │   │
│  │  - HintSystem*      m_hintSystem                             │   │
│  │  - Leaderboard*     m_leaderboard                            │   │
│  │  - PauseOverlay*    m_pauseOverlay                           │   │
│  │  - ThemeManager*    (Singleton access)                       │   │
│  └─────────────────────────────────────────────────────────────┘   │
└───────────────────────────────┬─────────────────────────────────────┘
                                │ uses (không kế thừa)
                                ▼
┌─────────────────────────────────────────────────────────────────────┐
│                         CORE LAYER                                  │
│                   (Legacy Components)                               │
│  ┌───────────────┐  ┌───────────────┐  ┌───────────────┐          │
│  │   GameBoard   │  │  MineCounter  │  │   MineTimer   │          │
│  │   (Legacy)    │  │   (Legacy)    │  │   (Legacy)    │          │
│  └───────────────┘  └───────────────┘  └───────────────┘          │
│  ┌───────────────┐  ┌───────────────┐  ┌───────────────┐          │
│  │     Tile      │  │   HighScore   │  │   GameStats   │          │
│  │   (Legacy)    │  │   (Legacy)    │  │   (Legacy)    │          │
│  └───────────────┘  └───────────────┘  └───────────────┘          │
└─────────────────────────────────────────────────────────────────────┘
```

### 4.2. Nguyên tắc thiết kế

#### 4.2.1. Open/Closed Principle

```cpp
// Legacy code ĐÓNG với sửa đổi
class MainWindow : public QMainWindow {
    // Không sửa đổi
};

// Extension code MỞ với mở rộng
class MainWindowExt : public QMainWindow {
    // Thêm mới mà không sửa MainWindow
};
```

#### 4.2.2. Composition over Inheritance

```cpp
// Thay vì kế thừa MainWindow, sử dụng Composition
class MainWindowExt : public QMainWindow {
private:
    // Sử dụng lại các component của legacy
    GameBoard*    m_gameBoard;      // Legacy component
    MineCounter*  m_mineCounter;    // Legacy component
    MineTimer*    m_mineTimer;      // Legacy component

    // Thêm extension components
    GameController* m_controller;    // New
    HintSystem*     m_hintSystem;   // New
    PauseOverlay*   m_pauseOverlay; // New
};
```

#### 4.2.3. Singleton Pattern (ThemeManager)

```cpp
class ThemeManager : public QObject {
public:
    static ThemeManager* instance() {
        static ThemeManager* s_instance = nullptr;
        if (!s_instance) {
            s_instance = new ThemeManager();
        }
        return s_instance;
    }

private:
    ThemeManager(QObject* parent = nullptr);
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;
};
```

### 4.3. Signal-Slot Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                     SIGNAL-SLOT CONNECTIONS                         │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  GameBoard                    MainWindowExt                         │
│  ┌──────────────┐            ┌──────────────┐                      │
│  │ initialized ─┼────────────►│ startGame() │                      │
│  │ victory ─────┼────────────►│ onVictory() │                      │
│  │ defeat ──────┼────────────►│ onDefeat()  │                      │
│  └──────────────┘            └──────────────┘                      │
│                                                                     │
│  GameController              MainWindowExt                          │
│  ┌──────────────┐            ┌──────────────┐                      │
│  │ pauseRequested┼───────────►│ showPause() │                      │
│  │ resumeRequested┼──────────►│ hidePause() │                      │
│  │ hintRequested ┼───────────►│ giveHint()  │                      │
│  │ timeExpired ──┼───────────►│ onTimeUp()  │                      │
│  └──────────────┘            └──────────────┘                      │
│                                                                     │
│  ThemeManager                 All Widgets                           │
│  ┌──────────────┐            ┌──────────────┐                      │
│  │ themeChanged ─┼───────────►│ updateStyle()│                     │
│  └──────────────┘            └──────────────┘                      │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

### 4.4. State Machine mở rộng

```
┌─────────────────────────────────────────────────────────────────────┐
│                    EXTENDED STATE MACHINE                           │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│                        ┌─────────────┐                              │
│                        │  NotStarted │◄─────────────────────────┐  │
│                        │   State     │                          │  │
│                        └──────┬──────┘                          │  │
│                               │ startGame()                     │  │
│                               ▼                                 │  │
│   ┌─────────────┐      ┌─────────────┐      ┌─────────────┐    │  │
│   │   Paused    │◄─────┤   Playing   │─────►│    Won      │    │  │
│   │   State     │      │   State     │      │   State     │    │  │
│   │   (NEW!)    │─────►└──────┬──────┘      └──────┬──────┘    │  │
│   └─────────────┘             │                    │           │  │
│    pause/resume               │ defeat()           │ newGame() │  │
│                               ▼                    │           │  │
│                        ┌─────────────┐             │           │  │
│                        │    Lost     │─────────────┴───────────┘  │
│                        │   State     │                             │
│                        └─────────────┘                             │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

---

## 5. Chi tiết triển khai

### 5.1. Cấu trúc thư mục dự án

```
minesweeper/
├── CMakeLists.txt               # Build configuration
├── src/
│   ├── main.cpp                 # Entry point (Modified)
│   │
│   │── [LEGACY FILES - Không sửa đổi hoặc sửa ít]
│   ├── mainwindow.h/cpp         # Original MainWindow
│   ├── gameboard.h/cpp          # Game board (thêm tileAt accessor)
│   ├── tile.h/cpp               # Tile class (thêm font-size)
│   ├── mineCounter.h/cpp        # Mine counter display
│   ├── minetimer.h/cpp          # Timer display
│   ├── highScore.h/cpp          # High score data
│   ├── highScoreModel.h/cpp     # High score model
│   ├── highScoreDialog.h/cpp    # High score dialog
│   ├── gameStats.h/cpp          # Game statistics
│   ├── gameStatsDialog.h/cpp    # Statistics dialog
│   ├── versionChecker.h/cpp     # Version checker
│   │
│   │── [EXTENSION FILES - Mới hoàn toàn]
│   ├── gameEnums.h              # NEW: Enum definitions
│   ├── gameController.h/cpp     # NEW: Game state controller
│   ├── themeManager.h/cpp       # NEW: Theme management
│   ├── startMenuDialog.h/cpp    # NEW: Start menu dialog
│   ├── customGameDialog.h/cpp   # NEW: Custom game dialog
│   ├── pauseOverlay.h/cpp       # NEW: Pause overlay
│   ├── hintSystem.h/cpp         # NEW: Hint system
│   ├── leaderboard.h/cpp        # NEW: Extended leaderboard
│   ├── leaderboardDialog.h/cpp  # NEW: Leaderboard dialog
│   └── mainwindow_ext.h/cpp     # NEW: Extended MainWindow
│
├── resources/
│   ├── resources.qrc            # Qt resource file
│   ├── icons/                   # Icon files
│   ├── images/                  # Image files
│   └── stylesheets/             # QSS stylesheets
│
└── build/                       # Build output directory
```

### 5.2. Các sửa đổi nhỏ trên Legacy Code

#### 5.2.1. gameboard.h - Thêm accessor

```cpp
// Thêm public method để HintSystem có thể truy cập tiles
public:
    Tile* tileAt(int row, int col) const {
        if (row >= 0 && row < m_rows && col >= 0 && col < m_cols) {
            return m_tiles[row][col];
        }
        return nullptr;
    }
```

**Lý do:** HintSystem cần truy cập tiles để tìm ô an toàn cho gợi ý.

#### 5.2.2. tile.cpp - Thêm font-size

```cpp
// Trước
setStyleSheet("Tile[state=\"1\"] { color: blue; ... }");

// Sau
setStyleSheet("Tile[state=\"1\"] { color: blue; font-size: 20px; ... }");
```

**Lý do:** Số hiển thị quá nhỏ trên màn hình độ phân giải cao.

#### 5.2.3. main.cpp - Sử dụng MainWindowExt

```cpp
// Trước
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}

// Sau
#include "mainwindow_ext.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindowExt window;  // Sử dụng extended version
    window.show();
    return app.exec();
}
```

### 5.3. Triển khai GameController

```cpp
// gameController.h
class GameController : public QObject {
    Q_OBJECT

public:
    explicit GameController(QObject* parent = nullptr);

    // State management
    GameEnums::GameState currentState() const { return m_state; }
    void startGame();
    void pauseGame();
    void resumeGame();
    void winGame();
    void loseGame();
    void resetGame();

    // Mode management
    GameEnums::GameMode currentMode() const { return m_mode; }
    void setGameMode(GameEnums::GameMode mode);

    // Difficulty management
    GameEnums::Difficulty currentDifficulty() const { return m_difficulty; }
    void setDifficulty(GameEnums::Difficulty difficulty);

    // Custom settings
    void setCustomSettings(int rows, int cols, int mines);
    CustomGameSettings customSettings() const { return m_customSettings; }

    // Theme management
    GameEnums::Theme currentTheme() const { return m_theme; }
    void setTheme(GameEnums::Theme theme);

    // Hints
    int hintsRemaining() const { return m_hintsRemaining; }
    void useHint();
    void resetHints();

    // Time Attack
    void startTimeAttack();
    void stopTimeAttack();
    int timeRemaining() const { return m_timeRemaining; }

signals:
    void stateChanged(GameEnums::GameState state);
    void modeChanged(GameEnums::GameMode mode);
    void difficultyChanged(GameEnums::Difficulty difficulty);
    void themeChanged(GameEnums::Theme theme);
    void pauseRequested();
    void resumeRequested();
    void hintRequested();
    void hintsRemainingChanged(int remaining);
    void timeAttackStarted(int seconds);
    void timeRemainingChanged(int seconds);
    void timeExpired();

private slots:
    void onTimeAttackTick();

private:
    void transitionTo(GameEnums::GameState newState);

    GameEnums::GameState m_state = GameEnums::GameState::NotStarted;
    GameEnums::GameMode m_mode = GameEnums::GameMode::Normal;
    GameEnums::Difficulty m_difficulty = GameEnums::Difficulty::Beginner;
    GameEnums::Theme m_theme = GameEnums::Theme::Light;

    CustomGameSettings m_customSettings{10, 10, 15};
    int m_hintsRemaining = 3;
    int m_timeRemaining = 0;

    QTimer* m_timeAttackTimer;
};
```

### 5.4. Triển khai StartMenuDialog

```cpp
// startMenuDialog.h
class StartMenuDialog : public QDialog {
    Q_OBJECT

public:
    enum class StartMenuContext {
        MainMenu,      // Hiển thị trang chính
        ModeSelection  // Hiển thị chọn mode (khi từ New Game)
    };

    explicit StartMenuDialog(QWidget* parent = nullptr,
                            StartMenuContext context = StartMenuContext::MainMenu);

    // Results
    GameEnums::GameMode selectedMode() const { return m_selectedMode; }
    GameEnums::Difficulty selectedDifficulty() const { return m_selectedDifficulty; }
    bool isCustomGame() const { return m_isCustom; }
    int customRows() const { return m_customRows; }
    int customCols() const { return m_customCols; }
    int customMines() const { return m_customMines; }

signals:
    void leaderboardRequested();
    void statisticsRequested();
    void settingsRequested();

private slots:
    void onNewGameClicked();
    void onNormalModeClicked();
    void onTimeAttackModeClicked();
    void onDifficultySelected(GameEnums::Difficulty difficulty);
    void onCustomGameClicked();
    void onBackClicked();

private:
    void setupUI();
    void showMainPage();
    void showModeSelectionPage();
    void showDifficultyPage();
    void applyTheme();

    QStackedWidget* m_stackedWidget;
    QWidget* m_mainPage;
    QWidget* m_modePage;
    QWidget* m_difficultyPage;

    GameEnums::GameMode m_selectedMode;
    GameEnums::Difficulty m_selectedDifficulty;
    StartMenuContext m_context;

    bool m_isCustom = false;
    int m_customRows, m_customCols, m_customMines;
};
```

### 5.5. Build Configuration (CMakeLists.txt)

```cmake
# Thêm các file mở rộng vào target
set(SOURCE_FILES
    # Legacy files
    src/main.cpp
    src/gameboard.cpp
    src/tile.cpp
    src/mainwindow.cpp
    src/mineCounter.cpp
    src/minetimer.cpp
    src/highScore.cpp
    src/highScoreModel.cpp
    src/highScoreDialog.cpp
    src/gameStats.cpp
    src/gameStatsDialog.cpp
    src/versionChecker.cpp

    # Extension files (NEW)
    src/gameController.cpp
    src/themeManager.cpp
    src/startMenuDialog.cpp
    src/customGameDialog.cpp
    src/pauseOverlay.cpp
    src/hintSystem.cpp
    src/leaderboard.cpp
    src/leaderboardDialog.cpp
    src/mainwindow_ext.cpp
)
```

---

## 6. Kết quả và đánh giá

### 6.1. Kết quả đạt được

#### 6.1.1. Tính năng hoàn thành

| #   | Tính năng            | Trạng thái    | Mô tả                          |
| --- | -------------------- | ------------- | ------------------------------ |
| 1   | Start Menu Dialog    | ✅ Hoàn thành | Giao diện khởi động hiện đại   |
| 2   | Mode Selection       | ✅ Hoàn thành | Chọn Normal/Time Attack        |
| 3   | Custom Game          | ✅ Hoàn thành | Tùy chỉnh kích thước và số mìn |
| 4   | Time Attack Mode     | ✅ Hoàn thành | Chế độ đếm ngược thời gian     |
| 5   | Pause System         | ✅ Hoàn thành | Tạm dừng với overlay che bảng  |
| 6   | Hint System          | ✅ Hoàn thành | 3 gợi ý ô an toàn mỗi game     |
| 7   | Theme System         | ✅ Hoàn thành | Light/Dark theme               |
| 8   | Extended Leaderboard | ✅ Hoàn thành | Lọc theo mode và difficulty    |

#### 6.1.2. Thống kê code

| Loại               | Số file  | Số dòng code |
| ------------------ | -------- | ------------ |
| Legacy (không đổi) | 14 files | ~2500 lines  |
| Legacy (sửa nhỏ)   | 3 files  | +10 lines    |
| Extension (mới)    | 12 files | ~3300 lines  |
| **Tổng cộng**      | 29 files | ~5800 lines  |

### 6.2. Screenshots

#### 6.2.1. Start Menu

```
╔═══════════════════════════════════════════════════╗
║                   MINESWEEPER                     ║
║                                                   ║
║            ┌─────────────────────┐                ║
║            │     🎮 NEW GAME     │                ║
║            └─────────────────────┘                ║
║            ┌─────────────────────┐                ║
║            │   🏆 LEADERBOARD    │                ║
║            └─────────────────────┘                ║
║            ┌─────────────────────┐                ║
║            │   📊 STATISTICS     │                ║
║            └─────────────────────┘                ║
║            ┌─────────────────────┐                ║
║            │    ⚙️  SETTINGS     │                ║
║            └─────────────────────┘                ║
║            ┌─────────────────────┐                ║
║            │      🚪 EXIT        │                ║
║            └─────────────────────┘                ║
║                                                   ║
╚═══════════════════════════════════════════════════╝
```

#### 6.2.2. Game Interface (Light Theme)

```
╔═══════════════════════════════════════════════════╗
║  Game  View  Help                                 ║
╠═══════════════════════════════════════════════════╣
║                                                   ║
║   ┌─────┐    ┌─────┐    ┌─────┐    [⏸] [💡]     ║
║   │ 010 │    │ 😊  │    │ 045 │     Hints: 3    ║
║   └─────┘    └─────┘    └─────┘                  ║
║                                                   ║
║   ┌───┬───┬───┬───┬───┬───┬───┬───┬───┐        ║
║   │ 1 │ 2 │   │   │ 1 │ 🚩│   │   │   │        ║
║   ├───┼───┼───┼───┼───┼───┼───┼───┼───┤        ║
║   │   │ 3 │ 2 │ 1 │   │ 2 │   │   │   │        ║
║   ├───┼───┼───┼───┼───┼───┼───┼───┼───┤        ║
║   │   │   │ ■ │ ■ │ ■ │ ■ │ ■ │   │   │        ║
║   ├───┼───┼───┼───┼───┼───┼───┼───┼───┤        ║
║   │   │   │ ■ │ ■ │ ■ │ ■ │ ■ │   │   │        ║
║   └───┴───┴───┴───┴───┴───┴───┴───┴───┘        ║
║                                                   ║
╚═══════════════════════════════════════════════════╝
```

#### 6.2.3. Time Attack Mode

```
╔═══════════════════════════════════════════════════╗
║  Game  View  Help                                 ║
╠═══════════════════════════════════════════════════╣
║                                                   ║
║   ┌─────┐    ┌─────┐    ┌─────┐                  ║
║   │ 010 │    │ 😊  │    │⏱02:30│  ← Đếm ngược   ║
║   └─────┘    └─────┘    └─────┘                  ║
║                                                   ║
║                  MODE: TIME ATTACK                ║
║                                                   ║
╚═══════════════════════════════════════════════════╝
```

### 6.3. Đánh giá

#### 6.3.1. Ưu điểm

| #   | Ưu điểm                 | Mô tả                                          |
| --- | ----------------------- | ---------------------------------------------- |
| 1   | **Tách biệt rõ ràng**   | Extension layer hoàn toàn độc lập với core     |
| 2   | **Không phá vỡ legacy** | Core gameplay vẫn hoạt động như cũ             |
| 3   | **Dễ bảo trì**          | Mỗi tính năng trong một module riêng           |
| 4   | **Mở rộng dễ dàng**     | Có thể thêm tính năng mới mà không sửa code cũ |
| 5   | **UI hiện đại**         | Start Menu cải thiện UX đáng kể                |

#### 6.3.2. Hạn chế

| #   | Hạn chế             | Nguyên nhân                                      |
| --- | ------------------- | ------------------------------------------------ |
| 1   | Duplicate code      | MainWindowExt có một số code tương tự MainWindow |
| 2   | Chưa tối ưu theme   | Một số component chưa style đầy đủ               |
| 3   | Testing chưa đầy đủ | Unit test cho extensions còn thiếu               |

#### 6.3.3. Hướng phát triển

1. **Multiplayer Mode:** Thêm chế độ chơi mạng
2. **Achievement System:** Hệ thống thành tựu
3. **Daily Challenge:** Thử thách hàng ngày
4. **Sound Effects:** Thêm âm thanh
5. **Animation:** Hiệu ứng animation mượt mà hơn

---

## 7. Kết luận

### 7.1. Tổng kết

Đồ án đã hoàn thành việc mở rộng ứng dụng Minesweeper với các tính năng mới trong khi **giữ nguyên hoàn toàn core gameplay** của hệ thống gốc. Phương pháp Extension Layer cho phép:

- Phát triển tính năng mới mà không ảnh hưởng đến code legacy
- Dễ dàng quay về phiên bản gốc nếu cần (chỉ đổi include trong main.cpp)
- Bảo trì và mở rộng độc lập

### 7.2. Bài học rút ra

1. **Hiểu rõ hệ thống trước khi mở rộng:** Phân tích kỹ architecture của legacy system
2. **Composition over Inheritance:** Ưu tiên sử dụng lại thay vì kế thừa
3. **Signal/Slot decoupling:** Giúp các component độc lập
4. **Singleton cho global state:** ThemeManager là ví dụ tốt
5. **Clear separation of concerns:** Mỗi class một trách nhiệm

### 7.3. Kỹ năng thu được

- Phân tích và làm việc với legacy code
- Thiết kế extension layer
- Qt Framework (Widgets, Signal/Slot, State Machine)
- CMake build system
- C++20 modern features

---

## 8. Tài liệu tham khảo

1. **Qt Documentation:** https://doc.qt.io/qt-6/
2. **Original Minesweeper Repository:** https://github.com/nholthaus/minesweeper
3. **C++ Reference:** https://en.cppreference.com/
4. **CMake Documentation:** https://cmake.org/documentation/
5. **Design Patterns:** Gang of Four - Design Patterns

---

## PHỤ LỤC

### A. Hướng dẫn build

```powershell
# Set environment
$env:PATH = "E:\minesweeper\Qt6\Tools\mingw1120_64\bin;E:\minesweeper\Qt6\6.7.2\mingw_64\bin"

# Configure
cd e:\minesweeper
cmake -B build -G "MinGW Makefiles"

# Build
cd build
mingw32-make -j8

# Run
.\bin\minesweeper.exe
```

### B. Cấu trúc Enum

```cpp
// gameEnums.h
namespace GameEnums {
    enum class GameState {
        NotStarted,  // Chưa bắt đầu
        Playing,     // Đang chơi
        Paused,      // Tạm dừng (NEW)
        Won,         // Thắng
        Lost         // Thua
    };

    enum class GameMode {
        Normal,      // Chế độ thường
        TimeAttack   // Đếm ngược (NEW)
    };

    enum class Difficulty {
        Beginner,
        Intermediate,
        Expert,
        Custom       // NEW
    };

    enum class Theme {
        Light,
        Dark
    };
}
```

### C. Bảng so sánh Legacy vs Extension

| Aspect            | Legacy (MainWindow)        | Extension (MainWindowExt)        |
| ----------------- | -------------------------- | -------------------------------- |
| **State Machine** | QStateMachine với 4 states | GameController với 5 states      |
| **Menus**         | Game/Difficulty/Help       | Game/View/Help + StartMenuDialog |
| **Difficulty**    | enum trong HighScore       | enum trong GameEnums             |
| **Mode**          | Không có                   | Normal/TimeAttack                |
| **Theme**         | Không có                   | Light/Dark                       |
| **Pause**         | Không có                   | PauseOverlay                     |
| **Hints**         | Không có                   | HintSystem (3 hints)             |
| **Leaderboard**   | HighScoreDialog            | LeaderboardDialog với filter     |

---

**--- HẾT BÁO CÁO ---**
