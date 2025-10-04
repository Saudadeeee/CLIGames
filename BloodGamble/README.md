# BloodGamble - Vietnamese Poker with Cheating Mechanics

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-green.svg)](https://github.com/Saudadeeee/CLIGames)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

## Tổng quan

**BloodGamble** là một game poker Texas Hold'em độc đáo với cơ chế gian lận sáng tạo. Người chơi đóng vai một kẻ gian lận chuyên nghiệp phải đối đầu với 3 AI "Hell Soldiers" (Quân lính địa ngục). Thay vì đặt tiền, tất cả đặt **HP (Health Points)** - khi hết HP sẽ bị loại khỏi game!

### Điểm đặc biệt:
- **HP-based betting**: Đặt cược bằng máu thay vì tiền
- **Advanced cheating system**: 7 loại gian lận với detection mechanics
- **Smart AI opponents**: AI với personality và adaptive behavior  
- **Colored interface**: Cards với màu sắc, menu số thân thiện
- **Vigilance system**: Hệ thống cảnh giác động làm game thử thách hơn

## Gameplay

### Luật chơi cơ bản:
- **Texas Hold'em** chuẩn với 2 hole cards + 5 community cards
- Mỗi player bắt đầu với **100 HP** (AI có 80 HP)
- Betting rounds: **Pre-flop → Flop → Turn → River → Showdown**
- Blinds: Small blind 1 HP, Big blind 2 HP

### Điều kiện thắng/thua:
- **Thắng**: Tất cả AI về 0 HP
- **Thua**: Player về 0 HP hoặc bị phát hiện gian lận quá nhiều

## Cài đặt & Chạy

### Yêu cầu hệ thống:
- **C++17** compiler (GCC, Clang, MSVC)
- **Make** hoặc **MinGW-make** (Windows)
- **Git** (để clone project)

### Compile & Run:

#### Windows (MinGW):
```bash
# Clone repository
git clone https://github.com/Saudadeeee/CLIGames.git
cd CLIGames/BloodGamble

# Build
mingw32-make clean
mingw32-make

# Run
cd build
./BloodGamble.exe
```

#### Linux/macOS:
```bash
# Clone repository  
git clone https://github.com/Saudadeeee/CLIGames.git
cd CLIGames/BloodGamble

# Build
make clean
make

# Run
cd build
./BloodGamble
```

#### Manual compilation:
```bash
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude \
    src/core/*.cpp src/game/*.cpp src/ai/*.cpp main.cpp \
    -o BloodGamble
```

## Hướng dẫn chơi

### Menu chính:
```
==================================================
YOUR TURN! Current bet to call: 2 HP
==================================================
1. Fold        - Bỏ bài
2. Call (2 HP) - Theo cược
3. Raise       - Tăng cược  
4. All-in      - Đặt tất cả HP
5. Cheat list  - Xem danh sách gian lận
6. Use cheat   - Sử dụng gian lận
7. Status      - Hiển thị trạng thái game
==================================================
Choose (1-7): 
```

### Hệ thống gian lận:

| Cheat | Mô tả | Detection Risk | HP Penalty | Cooldown |
|-------|-------|----------------|------------|----------|
| **SwapHands** | Đổi 2 lá bài với deck | 10% | 10 HP | 4 rounds |
| **PeekOpponentHole** | Nhìn lén bài AI | 6% | 5 HP | 2 rounds |
| **MuckSwap** | Thay đổi lá board tiếp theo | 18% | 15 HP | 5 rounds |
| **ForceFold** | Làm AI dễ fold | 12% | 8 HP | 4 rounds |
| **StackPeek** | Xem thông tin AI | 5% | 4 HP | 3 rounds |
| **CardMarking** | Bias luck cho card draw | 15% | 12 HP | 6 rounds |
| **BluffBoost** | Làm tất cả AI thận trọng | 8% | 6 HP | 3 rounds |

### Vigilance System:
- **Vigilance** tăng khi: Player thắng nhiều, bị phát hiện cheat
- **Vigilance** giảm khi: AI thắng, thời gian trôi qua
- **High vigilance** = Xác suất phát hiện cheat cao hơn
- **Risk management** là chìa khóa thành công!

## Kiến trúc Project

### Cấu trúc thư mục:
```
BloodGamble/
├── include/
│   ├── core/          # Core game components
│   │   ├── Card.h        # Card representation & Deck
│   │   ├── Player.h      # Player data structure  
│   │   ├── HandEvaluator.h # Poker hand evaluation
│   │   └── Config.h      # Game constants
│   ├── game/          # Game logic
│   │   ├── GameState.h   # Game state management
│   │   ├── CheatSystem.h # Cheating mechanics
│   │   └── BloodGambleGame.h # Main game controller
│   └── ai/
│       └── AIPlayer.h    # AI decision making
├── src/              # Implementation files
│   ├── core/
│   ├── game/
│   └── ai/
├── build/            # Compiled binaries
├── docs/             # Documentation
├── main.cpp             # Entry point
├── Makefile             # Build configuration
└── README.md            # This file
```

### Design Patterns sử dụng:
- **Modular Architecture**: Tách biệt concerns rõ ràng
- **Strategy Pattern**: AI behavior switching
- **Observer Pattern**: Vigilance system monitoring
- **Command Pattern**: Player actions handling
- **Factory Pattern**: Cheat types creation

## AI System

### AI Personalities:
- **Aggression**: 0.0-1.0 (Độ hung hăng)
- **Tightness**: 0.0-1.0 (Độ thận trọng)  
- **Suspicion**: 0.0-1.0 (Độ nghi ngờ)
- **Adaptivity**: AI học hỏi từ player behavior

### Decision Making:
- **Pre-flop**: Hand strength evaluation
- **Post-flop**: Board texture analysis
- **Betting**: Pot odds calculation
- **Bluff detection**: Pattern recognition

## Features

### Đã hoàn thành:
- [x] Complete Texas Hold'em implementation
- [x] 7 unique cheating mechanics
- [x] Dynamic vigilance system
- [x] Colored card display
- [x] Number-based menu system
- [x] Winning hand descriptions
- [x] Modular code architecture
- [x] Cross-platform compatibility

### Có thể mở rộng:
- [ ] Multiplayer online support
- [ ] Tournament mode
- [ ] Achievement system
- [ ] Hand history replay
- [ ] Custom AI difficulty levels
- [ ] Statistics tracking
- [ ] Sound effects

## Troubleshooting

### Common issues:

**Build errors:**
```bash
# Make sure you have C++17 support
g++ --version

# Create directories if missing
mkdir -p build/obj/src/{core,game,ai}
```

**Runtime errors:**
```bash
# Run with seed for debugging
./BloodGamble
Enter seed: 12345  # Use same seed to reproduce
```

**Performance issues:**
- Game is optimized for single-threaded performance
- Memory usage ~5MB
- No external dependencies required

## Contributing

Contributions welcome! Areas for improvement:
- **Balance tuning**: Cheat detection probabilities
- **AI enhancement**: More sophisticated decision making
- **UI/UX**: Better visual feedback
- **Performance**: Optimization opportunities
- **Testing**: Unit tests for core logic

## Development Notes

### Code Style:
- **C++17** standard features
- **RAII** for resource management
- **const-correctness** throughout
- **Clear naming** conventions

### Testing:
```bash
# Test with fixed seed
./BloodGamble
Enter seed: 0  # Random seed
Enter seed: 42 # Fixed seed for testing
```

### Debug Mode:
- Use `#define DEBUG` for verbose logging
- Check vigilance calculations
- Monitor AI decision trees

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Author

**Saudadeeee** - [GitHub Profile](https://github.com/Saudadeeee)

---

*Game được phát triển với tình yêu dành cho poker và passion về game mechanics sáng tạo!*

**Enjoy the thrill of cheating without real-world consequences!**