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
                    printf("=================== DANH SACH SACH CO TRONG THU VIEN KAT=============================\n");
                    display_tree(thu_vien);
                    break;

                case 2: //2. Them sach moi vao he thong
                    printf("\n--- CHUC NANG: THEM SACH MOI ---\n\n");
                    if (authenticateAdmin()) { // Đúng mật khẩu mới cho thêm
                        thu_vien = insertnode(thu_vien);
                        save_database(thu_vien); 
                    }
                    break;

                case 3: //3. Mượn / Trả sách 
                    printf("\n--- CHUC NANG: MUON / TRA SACH ---\n\n");
                    borrow_return_book(thu_vien);
                    break;

                case 4: //4. Tim kiem sach theo Ma ID
                    printf("\n--- CHUC NANG: TIM KIEM SACH ---\n\n");
                    printf(" Nhap Ma ID can tim: ");
                    search();
                    printf("\n [Ket qua]: Tim thay sach!\n");
                    break;

                case 5: //5. Sua thong tin sach
                    printf("\n--- CHUC NANG: SUA TEN SACH ---\n\n");
                    if (authenticateAdmin()) { // Đúng mật khẩu mới cho sửa
                        change_inf_book(thu_vien);
                        save_database(thu_vien);
                    }
                    break;

                case 6:{ //6. Xoa sach khoi he thong
                printf("\n--- CHUC NANG: XOA SACH KHOI HE THONG ---\n\n");
                if (authenticateAdmin()) { // Đúng mật khẩu mới cho xóa
                        int deleteId;
                        printf(" Nhap Ma ID sach can xoa: ");
                        if (scanf("%d", &deleteId) == 1) {
                            clear_buffer();
                            if (search_id(deleteId, thu_vien) != NULL) {
                                thu_vien = deleteBook(deleteId, thu_vien);
                                save_database(thu_vien);
                                printf("\n [Thong bao]: Xoa thanh cong va da cap nhat database.dat!\n");
                            } else {
                                printf("\n [Loi]: Khong tim thay sach co ID %d de xoa!\n", deleteId);
                            }
                        } else {
                            printf("\n [Loi]: Ma ID nhap vao khong hop le!\n");
                            clear_buffer();
                        }
                    }
                    break;
                }

                case 7: //7. Xuat du lieu ra file CSV
                    printf("\n--- CHUC NANG: XUAT DU LIEU RA FILE CSV ---\n\n");
                    export_file(thu_vien);
                    break;
                case 8: //8. Thoát
                    printf("\n  ==================================================\n");
                    printf("     Cam on ban da su dung ung dung cua KAT! \n");
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
