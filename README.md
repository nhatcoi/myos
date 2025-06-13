# Tính số Pi bằng phương pháp Monte Carlo với MPI

## Mô tả

Chương trình này sử dụng phương pháp Monte Carlo để ước tính giá trị của số Pi bằng cách:
1. Gieo ngẫu nhiên N điểm vào hình vuông có cạnh 2R
2. Đếm số điểm m rơi vào trong hình tròn nội tiếp có bán kính R
3. Tính Pi = 4 * m / N

Chương trình sử dụng MPI để song song hóa tính toán với:
- `MPI_Bcast`: Phân phối số lượng điểm N
- `MPI_Reduce`: Tổng hợp kết quả từ các process

## Yêu cầu hệ thống

- Trình biên dịch C (gcc)
- Thư viện MPI (OpenMPI hoặc MPICH)

### Cài đặt MPI trên macOS:
```bash
# Sử dụng Homebrew
brew install open-mpi

# Hoặc sử dụng MacPorts
sudo port install openmpi
```

### Cài đặt MPI trên Ubuntu/Debian:
```bash
sudo apt-get install libopenmpi-dev openmpi-bin
```

## Biên dịch

```bash
# Sử dụng Makefile
make

# Hoặc biên dịch trực tiếp
mpicc -Wall -O2 -lm -o pi_monte_carlo nvnhat1.c
```

## Chạy chương trình

```bash
# Chạy với 4 processes
mpirun -np 4 ./pi_monte_carlo

# Chạy với 8 processes
mpirun -np 8 ./pi_monte_carlo

# Hoặc sử dụng Makefile
make run      # Chạy với 4 processes
make run8     # Chạy với 8 processes
```

## Kết quả mẫu

```
=== TÍNH SỐ PI BẰNG PHƯƠNG PHÁP MONTE CARLO ===
Số processes: 4
Tổng số điểm gieo: 1000000
Số điểm rơi vào hình tròn: 785263
Tỷ lệ điểm trong hình tròn: 0.785263
Pi ước tính: 3.1410520000
Pi thực tế: 3.1415926536
Sai số: 0.0005406536
Sai số tương đối: 0.017207%
Thời gian thực hiện: 0.023456 giây

=== THÔNG TIN PHÂN PHỐI CÔNG VIỆC ===
Điểm/process (trung bình): 250000

Process 0: Xử lý 250000 điểm, tìm được 196394 điểm trong hình tròn
Process 1: Xử lý 250000 điểm, tìm được 196128 điểm trong hình tròn
Process 2: Xử lý 250000 điểm, tìm được 196456 điểm trong hình tròn
Process 3: Xử lý 250000 điểm, tìm được 196285 điểm trong hình tròn
```

## Giải thích thuật toán

1. **Nguyên lý**: Hình tròn bán kính R nội tiếp trong hình vuông cạnh 2R
   - Diện tích hình tròn: π × R²
   - Diện tích hình vuông: (2R)² = 4R²
   - Tỷ lệ diện tích: π/4

2. **Monte Carlo**: Gieo ngẫu nhiên N điểm vào hình vuông
   - Điểm (x,y) trong hình tròn nếu: x² + y² ≤ R²
   - Tỷ lệ điểm trong hình tròn ≈ π/4
   - Suy ra: π ≈ 4 × (số điểm trong hình tròn) / N

3. **Song song hóa với MPI**:
   - Chia N điểm cho các process
   - Mỗi process tính số điểm trong hình tròn của phần mình
   - Tổng hợp kết quả bằng MPI_Reduce

## Độ chính xác

Độ chính xác phụ thuộc vào:
- Số lượng điểm N (càng lớn càng chính xác)
- Chất lượng bộ sinh số ngẫu nhiên
- Số lượng process (nhiều process → ít tương quan)

Với N = 10⁶, sai số thường < 0.1%