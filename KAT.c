#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "interface.c"
#include "core_tree.c"


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
                case 1: // 1. Xem danh sach sach hien co
                    display_tree(thu_vien);
                    break;

                case 2: //2. Them sach moi vao he thong
                    printf("\n--- CHUC NANG: THEM SACH MOI ---\n\n");
                    insertnode(thu_vien);
                    printf("\n [Thong bao]: Them thanh cong vao file .dat!\n");
                    break;

                case 3: //3. Tim kiem sach theo Ma ID
                    printf("\n--- CHUC NANG: TIM KIEM SACH ---\n\n");
                    printf(" Nhap Ma ID can tim: ");
                    search();
                    printf("\n [Ket qua]: Tim thay sach!\n");
                    break;

                case 4: //4. Sua thong tin sach
                    printf("\n--- CHUC NANG: SUA TEN SÁCH ---\n\n");
                    change_inf_book(thu_vien);
                    save_database(thu_vien);
                    break;

                case 5: //5. Xoa sach khoi he thong
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
