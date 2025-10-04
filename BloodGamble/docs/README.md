# BloodGamble - Poker Game with Cheating Mechanics

## Tổng quan
BloodGamble là một game poker Texas Hold'em với cơ chế cheat độc đáo. Người chơi đóng vai một kẻ gian lận giỏi phải đối đầu với 3 AI "quân lính địa ngục". Thay vì đặt tiền, mọi người đặt HP (máu) - khi hết HP thì bị loại khỏi game.

## Cấu trúc File

### Core System Files
- **main.cpp** - Entry point của game
- **Config.h** - Các hằng số và enum configuration

### Card & Hand System
- **Card.h/cpp** - Card struct và Deck class
- **HandEvaluator.h/cpp** - Đánh giá sức mạnh bài poker

### Player System
- **Player.h/cpp** - Player class cho cả human và AI
- **AIPlayer.h/cpp** - Logic AI decision making

### Cheat System
- **CheatSystem.h/cpp** - Quản lý các loại cheat và effects
- **GameState.h/cpp** - Game state management và cheat execution

### Game Engine
- **BloodGambleGame.h/cpp** - Main game loop và betting rounds

## Cách Compile

### Sử dụng Makefile (Windows với make):
```bash
make all
```

### Manual compilation:
```bash
g++ -std=c++17 -c *.cpp
g++ -std=c++17 -o BloodGamble.exe *.o
```

### Clean object files:
```bash
make clean
```

## Cách Chơi

### Commands trong game:
- `fold` - Bỏ bài
- `call` - Theo cược
- `raise <amount>` - Tăng cược
- `allin` - Đặt tất cả HP
- `cheat list` - Hiển thị danh sách cheat
- `cheat use <name> [target]` - Sử dụng cheat
- `status` - Hiển thị trạng thái game

### Các loại Cheat:
1. **SwapHands** - Đổi bài trên tay với bài random từ deck
2. **PeekOpponentHole** - Nhìn lén bài của AI
3. **MuckSwap** - Thay đổi lá tiếp theo trên board
4. **ForceFold** - Làm AI dễ fold hơn
5. **StackPeek** - Xem thông tin AI (HP, tính cách)
6. **CardMarking** - Bias luck cho card tiếp theo
7. **BluffBoost** - Làm tất cả AI thận trọng hơn

### Hệ thống Vigilance:
- Vigilance tăng khi player thắng nhiều
- Vigilance tăng khi bị phát hiện cheat
- Vigilance cao = xác suất phát hiện cheat cao hơn
- Vigilance giảm khi AI thắng

## Mechanics

### HP System:
- Player bắt đầu với 100 HP
- Mỗi AI bắt đầu với 80 HP
- Khi đặt cược = mất HP tạm thời vào pot
- Thắng = nhận tất cả HP trong pot
- Thua = mất HP vĩnh viễn

### Detection System:
- Mỗi cheat có base detection probability
- Detection probability bị ảnh hưởng bởi:
  - Global vigilance level
  - Target AI suspicion level
  - Game stage (pre-flop vs river)
  - Repeat usage penalty

### Victory Conditions:
- **Player thắng**: Tất cả AI về 0 HP
- **Player thua**: Player về 0 HP

## Dependencies
- C++17 compiler (g++, clang++, MSVC)
- Standard library only (không cần external libraries)

## Architecture Notes

### Modular Design:
- Mỗi component được tách riêng file để dễ debug
- Header files chứa interface, cpp files chứa implementation
- Dependency injection pattern cho game state

### Extensibility:
- Dễ dàng thêm cheat mới trong CheatSystem
- AI behavior có thể điều chỉnh qua parameters
- Hand evaluation có thể mở rộng cho variants khác

### Error Handling:
- Exception handling cho deck operations
- Input validation trong player actions
- Boundary checking cho HP calculations

## Development Tips

### Debug Mode:
- Thêm debug prints trong các effect functions
- Sử dụng fixed seed để reproduce bugs
- Log vigilance changes để tuning balance

### Balancing:
- Điều chỉnh base detection probabilities trong CheatSystem.cpp
- Thay đổi vigilance increment/decrement trong Config.h
- Tuning AI personality parameters

### Adding New Cheats:
1. Thêm CheatType vào CheatSystem::initializeCheatTypes()
2. Implement effect function
3. Set appropriate detection probability và cooldown
4. Test với different vigilance levels

## Performance
- Game sử dụng efficient card evaluation
- Memory footprint nhỏ (~few MB)
- Single-threaded design cho deterministic behavior
- Fast enough cho real-time play

Game được thiết kế để có thể mở rộng dễ dàng với thêm nhiều features như multiplayer, tournaments, hoặc different game modes.