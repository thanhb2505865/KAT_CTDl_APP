#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include<string.h>

#define MAX_OPTIONS 6

// Hàm di chuyển con trỏ chuột trên Console để vẽ không bị nhấp nháy
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Hàm tô màu: textColor (màu chữ), bgColor (màu nền)
void setColor(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

// Hàm ẩn con trỏ nhấp nháy trên màn hình giúp giao diện chuyên nghiệp hơn
void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// Vẽ giao diện Menu chính dựa vào dòng đang được chọn (currentChoice)
void drawMenu(int currentChoice) {
    char *options[MAX_OPTIONS] = {
        "1. Xem danh sach sach hien co",
        "2. Them sach moi vao he thong",
        "3. Tim kiem sach theo Ma ID",
        "4. Sua thong tin sach",
        "5. Xoa sach khoi he thong",
        "6. Thoat chuong trinh"
    };

    gotoxy(0, 0); // Luôn vẽ đè từ góc trên cùng, tránh dùng system("cls") liên tục gây lag
    
    setColor(11, 0); // Màu xanh dương nhạt cho tiêu đề
    printf("\n ==================================================================================\n");
    printf("                      HE THONG QUAN LY THU VIEN KAT XIN CHAO\n");
    printf(" ===================================================================================\n");
    
    setColor(10, 0); // Màu xanh lá cho trạng thái
    printf("   [Trang thai]: Database.dat da ket noi vao RAM \n");
    setColor(7, 0);  // Trả về màu trắng mặc định
    printf("  --------------------------------------------------\n\n");

    // Vòng lặp vẽ các lựa chọn menu
    for (int i = 0; i < MAX_OPTIONS; i++) {
        if (i == currentChoice) {
            // Dòng đang chọn: Đổi nền xanh dương (1), chữ trắng sáng (15)
            printf("   "); 
            setColor(15, 1); 
            printf(" > %-40s ", options[i]);
            setColor(7, 0); // Reset ngay sau khi in xong
            printf("\n\n");
        } else {
            // Dòng bình thường
            printf("     %-42s \n\n", options[i]);
        }
    }
    
    printf("  --------------------------------------------------\n");
    setColor(8, 0); // Màu xám cho hướng dẫn
    printf("   Dung phan phim [Thuong/Dien] de chon, [Enter] de vao.");
    setColor(7, 0);
}

// Hàm nhập mật khẩu hiển thị dấu *
void inputPassword(char *password, int maxLength) {
    int i = 0;
    char ch;
    while (1) {
        ch = getch(); // Đọc từng phím bấm mà không in ra màn hình
        if (ch == 13) { // Phím ENTER (Xuống dòng) -> Hoàn thành
            password[i] = '\0';
            break;
        } else if (ch == 8) { // Phím BACKSPACE (Xóa ký tự)
            if (i > 0) {
                i--;
                printf("\b \b"); // Xóa 1 dấu * trên màn hình
            }
        } else if (ch >= 32 && ch <= 126 && i < maxLength - 1) { // Ký tự in được
            password[i++] = ch;
            printf("*"); // Thay ký tự thật bằng dấu *
        }
    }
    printf("\n");
}

// Hàm kiểm tra quyền Admin
int authenticateAdmin() {
    const char ADMIN_PASS[] = "123456"; // MẬT KHẨU CỦA QUẢN LÝ (Có thể thay đổi)
    char inputPass[50];

    printf("\n [YEU CAU QUYEN QUAN LY]\n");
    printf(" Nhap mat khau Admin: ");
    inputPassword(inputPass, 50);

    if (strcmp(inputPass, ADMIN_PASS) == 0) {
        printf(" >> XAC THUC THANH CONG! Cho phep truy cap.\n\n");
        return 1; // Đúng mật khẩu
    } else {
        printf(" >> [LOI] Mat khau khong dung! Ban khong co quyen thuc hien thao tac nay.\n");
        return 0; // Sai mật khẩu
    }
}
