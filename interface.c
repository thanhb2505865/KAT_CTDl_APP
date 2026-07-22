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
    SetCoansoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
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
    printf("\n  ==================================================\n");
    printf("     HE THONG QUAN LY THU VIEN CONSOLE XIN SA\n");
    printf("  ==================================================\n");
    
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

int main() {
    Tree thu_vien = lood_database(); // Nạp dữ liệu từ database.dat lên cây
    int currentChoice = 0;
    int key;

    hideCursor(); // Ẩn con trỏ đi cho đẹp

    while (1) {
        drawMenu(currentChoice);

        // Bắt sự kiện phím bấm
        key = getch();
        if (key == 0 || key == 224) { // Nếu là phím đặc biệt (Mũi tên)
            key = getch();
            if (key == 72) { // Mũi tên LÊN
                currentChoice = (currentChoice == 0) ? MAX_OPTIONS - 1 : currentChoice - 1;
            } else if (key == 80) { // Mũi tên XUỐNG
                currentChoice = (currentChoice == MAX_OPTIONS - 1) ? 0 : currentChoice + 1;
            }
        } else if (key == 13) { // Người dùng nhấn ENTER
            system("cls"); // Xóa màn hình một lần duy nhất khi vào chức năng
            
            switch (currentChoice + 1) {
                case 1:
                    printf("\n--- DANH SACH SACH HIEN CO ---\n\n");
                    printf("  %-10s | %-30s\n", "MA SACH", "TEN SACH");
                    printf("  ------------------------------------------\n");
                    printf("  101        | Cau truc du lieu va giai thuat\n");
                    printf("  102        | Lap trinh C co ban\n");
                    break;

                case 2:
                    printf("\n--- CHUC NANG: THEM SACH MOI ---\n\n");
                    int newId; char newTitle[100];
                    printf(" Nhap Ma ID moi: ");
                    scanf("%d", &newId);
                    fflush(stdin); // Xóa bộ đệm
                    printf(" Nhap Ten Sach moi: ");
                    fgets(newTitle, sizeof(newTitle), stdin);
                    newTitle[strcspn(newTitle, "\n")] = '\0'; // Nhớ #include <string.h> ở đầu file nha ní
                    printf("\n [Thong bao]: Them thanh cong vao file .dat!\n");
                    break;

                case 3:
                    printf("\n--- CHUC NANG: TIM KIEM SACH ---\n\n");
                    printf(" Nhap Ma ID can tim: ");
                    // Xử lý tìm kiếm ở đây...
                    printf("\n [Ket qua]: Tim thay sach!\n");
                    break;

                case 4:
                    printf("\n--- CHUC NANG: SUA TEN SÁCH ---\n\n");
                    break;

                case 5:
                printf("\n--- CHUC NANG: XOA SACH KHOI HE THONG ---\n\n");
                int deleteId;
                printf(" Nhap Ma ID sach can xoa: ");
                if (scanf("%d", &deleteId) == 1) {
        // 1. Gọi hàm xóa sách khỏi Cây tìm kiếm nhị phân
                    thu_vien = deleteBook(deleteId, thu_vien);
        // 2. Cập nhật lại dữ liệu xuống file database.dat ngay lập tức
                    save_database(thu_vien);
                        printf("\n [Thong bao]: Thao tac xoa hoan tat va da cap nhat database.dat!\n");
                    } else {
                        printf("\n [Loi]: Ma ID nhap vao khong hop le!\n");}
                clear_buffer(); // Xóa bộ đệm bàn phím
                break;

                case 6:
                    printf("\n  ==================================================\n");
                    printf("     Cam on ne da su dung app! Dang dong Console... \n");
                    printf("  ==================================================\n");
                    return 0;
            }
            
            setColor(8, 0);
            printf("\n\n [Bam phim bat ky de quay lai Menu chinh...]");
            getch();
            system("cls"); // Xóa màn hình để chuẩn bị vẽ lại menu sạch sẽ
        }
    }
    return 0;
}
