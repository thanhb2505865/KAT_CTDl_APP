#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Định nghĩa trạng thái mượn sách bằng enum cho rõ nghĩa
typedef enum {
    AVAILABLE = 0,   // Chưa mượn (Có sẵn)
    BORROWED = 1     // Đã mượn
} BookStatus;

// 1. Cấu trúc thông tin của một cuốn Sách (đã thêm trạng thái)
typedef struct Book {
    int id;                 // Mã sách (Khóa)
    char title[100];        // Tên sách
    char author[50];        // Tác giả
    int year;               // Năm xuất bản
    BookStatus status;      // Trạng thái: AVAILABLE hoặc BORROWED
} Book;

// 2. Định nghĩa một Node trên Cây nhị phân
struct Node{
    Book data;              
    struct Node* left;  
    struct Node* right; 
};

typedef struct Node* Tree;

int convertTxtToDat(const char *txtFileName, const char *datFileName) {
    FILE *fTxt = fopen(txtFileName, "r");
    if (fTxt == NULL) {
        printf("[X] Loi: Khong mo đuoc file '%s'!\n", txtFileName);
        return 0;
    }

    FILE *fDat = fopen(datFileName, "wb");
    if (fDat == NULL) {
        printf("[X] Loi: Khong tao đuoc file '%s'!\n", datFileName);
        fclose(fTxt);
        return 0;
    }

    Book temp;
    char line[256];
    int count = 0;

    // Đọc từng dòng ID
    while (fgets(line, sizeof(line), fTxt) != NULL) {
        // 1. Lấy ID
        line[strcspn(line, "\r\n")] = 0; // Xóa \n
        if (strlen(line) == 0) continue;  // Bỏ qua dòng trống nếu có
        temp.id = atoi(line);            // Chuyển chuỗi thành số nguyên ID

        // 2. Lấy Tên sách
        if (fgets(temp.title, sizeof(temp.title), fTxt) != NULL) {
            temp.title[strcspn(temp.title, "\r\n")] = 0;
        }

        // 3. Lấy Tác giả
        if (fgets(temp.author, sizeof(temp.author), fTxt) != NULL) {
            temp.author[strcspn(temp.author, "\r\n")] = 0;
        }

        // 4. Lấy Năm xuất bản
        if (fgets(line, sizeof(line), fTxt) != NULL) {
            line[strcspn(line, "\r\n")] = 0;
            temp.year = atoi(line);       // Chuyển chuỗi thành số nguyên Year
        }

        // 5. Trạng thái mặc định
        temp.status = AVAILABLE;

        // Ghi struct vào file .dat
        fwrite(&temp, sizeof(Book), 1, fDat);
        count++;
    }

    fclose(fTxt);
    fclose(fDat);

    printf(">> [Thanh cong] Đa chuyen tron ven %d cuon sach sang '%s'!\n", count, datFileName);
    return 1;
}

// HÀM: readBook
// Tham số: Tree T, FILE* f;
// Mục đích: Đem dữ liệu từ cây trả về file 
void readBook(Tree T, FILE* f) {
    if(T == NULL) return;
    else {
        fwrite(&T->data, sizeof(Book), 1, f);
        readBook(T->left, f);
        readBook(T->right, f);
    }
}

// HÀM: save_database
// Tham số: Tree T
// Mục đích: Đem dữ liệu từ Tree T trả về cho file database.dat
void save_database(Tree T) {
    FILE* f = fopen("database.dat", "wb");
    if(T == NULL) return;
    else {
        readBook(T, f);
    }
    fclose(f);
}

// HÀM: insertbook
// Tham số: Book x, Tree Root
// Mục đích: Thêm Book x vào tham số Tree Root; Nếu có thì thêm , không thì trả về địa chỉ cây Root
// Đầu ra   : Trả về địa chỉ của cây tham số Root sao khi gắn book vào 
Tree insertbook(Book x, Tree Root) { 
    if(Root == NULL) {
        Tree New = (Tree)malloc(sizeof(struct Node));
        x.status = AVAILABLE;
        New->data = x;
        New->left = NULL;
        New->right = NULL;
        Root = New;
        return Root;
    }
    else {
        if(x.id > Root->data.id) {
            Root->right = insertbook(x, Root->right);
        }
        else if((x.id < Root->data.id) ){
            Root->left = insertbook(x, Root->left);
        }
    }
    return Root;
}

// HÀM: lood_database
// Mục đích: Đem dữ liệu từ file dựng thành cây trên Ram
// Đầu ra: Tree result sao khi dữ liệu trên file được đọc đưa lên cây
Tree lood_database() {
    Tree result = NULL;
    FILE* f = fopen("database.dat", "rb");
    if (f != NULL) {
        Book temp;
        while(fread(&temp, sizeof(Book), 1, f) == 1) {
        result = insertbook(temp, result);
        } 
        fclose(f);
    }
    else {
        if (convertTxtToDat("database.txt", "database.dat")) {
            result = lood_database();
        } else {
            printf(">> [Thong bao] Khong tim thay 'database.dat' hoac 'database.txt'. Khoi tao cay rong.\n");
        }
    }
    return result;
}

// Hàm duyệt cây in ra màn hình 
void display_tree(Tree thu_vien) {
    if (thu_vien != NULL) {
        display_tree(thu_vien->left);
        char* statusStr = (thu_vien->data.status == AVAILABLE) ? "Trong kho san sang" : "Dang muon";
        printf("%-5d | %-30s | %-30s | %-6d | %-18s\n", 
            thu_vien->data.id, thu_vien->data.title, thu_vien->data.author, thu_vien->data.year, statusStr);
        display_tree(thu_vien->right);
    }
}

// Hàm xóa bộ nhớ đệm (tránh lỗi trôi lệnh khi dùng scanf xong dùng fgets)
void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Ham doc Book
void scanfbook(Book* temp) {
    printf("Nhap ID (Ma sach): "); 
    scanf("%d", &temp->id);
    clear_buffer(); 
    printf("Nhap ten sach: "); 
    fgets(temp->title, 100, stdin); 
    temp->title[strcspn(temp->title, "\n")] = 0;
    printf("Nhap tac gia: "); 
    fgets(temp->author, 50, stdin); 
    temp->author[strcspn(temp->author, "\n")] = 0;
    printf("Nhap nam xuat ban: "); 
    scanf("%d", &temp->year);
    clear_buffer();
    temp->status = AVAILABLE;
}

// Ham print Book
void printbook(Book virtual) {
    char* statusStr = (virtual.status == AVAILABLE) ? "Trong kho san sang" : "Dang muon";
    printf("=================== THONG TIN SACH TRONG THU VIEN KAT=============================\n");
    printf("%-5d | %-30s | %-30s | %-6d | %-18s\n", 
        virtual.id, virtual.title, virtual.author, virtual.year, statusStr);
}

// Ham tim kiem tra id Book 
Tree search_id(int x, Tree Root) { //Khong thay tra ve NULL; Tra ve Tree // Minh chua hieu ham search
    if (Root == NULL) return NULL; // Truong hop tim khong thay x
    else {
        if(Root->data.id == x) {
            return Root;
        }
        else {
            if(x > Root->data.id) {
                return search_id(x, Root->right);
            }
            else if (x < Root->data.id) {
                return search_id(x, Root->left);
            }
        }
    }
}
// HÀM: insertnode
// Tham số: Book x, Tree Root
// Mục đích: 
// Đầu ra:
Tree insertnode(Tree thu_vien) { 
    Book b;
    printf("\n--- NHAP THONG TIN SACH MOI ---\n");
    scanfbook(&b);
    if (search_id(b.id, thu_vien) != NULL) {
        printf(">> [LOI] Sach co ID %d da ton tai! Khong the them trung ID.\n", b.id);
        return thu_vien; 
    }
    thu_vien = insertbook(b, thu_vien);
    printf(">> Da them sach '%s' (ID: %d) vao he thong thanh cong!\n", b.title, b.id);
    printf("\n [Thong bao]: Them thanh cong vao file .dat!\n");
    return thu_vien;
}


// Hàm trả về trạng thái của sách. Chưa mượn trả về 1; Đã mượn trả về 0
int search_status(Tree T) {
    return T->data.status == AVAILABLE;
}

// HÀM: search
// Mục đích: Tìm và in ra Book 
void search() {
    int x;
    printf("Nhap ID cuon sach ban tim kiem: ");
    scanf(" %d", &x);
    Tree result = lood_database();
    Tree test = search_id(x, result);
    if (test == NULL) {
        printf("Khong tim thay cuon sach ca ma so ID");
        return;
    }
    else {
        if(search_status(test)) {
            printf("=================== DANH SACH SACH CO TRONG THU VIEN KAT=============================\n");
            printf("%-5s | %-30s | %-30s | %-6s | %-18s\n", "ID", "Ten Sach", "Tac Gia", "Nam XB", "Trang Thai");
            printf("----------------------------------------------------------------------------------\n");
            printf("%-5d | %-30s | %-30s | %-6d | %-18s\n", test->data.id, test->data.title, test->data.author, test->data.year, "Trong kho san sang");
        }
        else {
            printf("=================== DANH SACH SACH CO TRONG THU VIEN KAT=============================\n");
            printf("%-5d | %-30s | %-30s | %-6d | %-18s\n", "ID", "Ten Sach", "Tac Gia", "Nam XB", "Trang Thai");
            printf("----------------------------------------------------------------------------------\n");
            printf("%-5d | %-30s | %-30s | %-6d | %-18s\n", test->data.id, test->data.title, test->data.author, test->data.year, "Dang muon");
        }
    }
}

// HÀM: change_inf_book
// Tham số: Tree thu_vien
// Mục đích: Thay doi cac thong tin cua Book trong Thu vien
void change_inf_book(Tree thu_vien) {
    Book* temp_book;
    printf("Nhap ID (Ma sach) ban muon thay doi: ");
    int temp;
    scanf("%d", &temp);
    Tree search = search_id(temp, thu_vien);
    if (search == NULL) {
        printf("Khong tim thay cuan sach ca ma so ID\n");
    }
    else {
        while (getchar() != '\n');
        temp_book->id = search->data.id;
        printf("Nhap ten sach: "); 
        fgets(temp_book->title, 100, stdin); 
        temp_book->title[strcspn(temp_book->title, "\n")] = 0;
        printf("Nhap tac gia: "); 
        fgets(temp_book->author, 50, stdin); 
        temp_book->author[strcspn(temp_book->author, "\n")] = 0;
        printf("Nhap nam xuat ban: "); 
        scanf("%d", &temp_book->year);
        temp_book->status = AVAILABLE;
        search->data = *temp_book;
        printf("Cap nhat du lieu sach thanh cong\n");
        char* statusStr = (temp_book->status == AVAILABLE) ? "Trong kho san sang" : "Dang muon";
        printf("=================== THONG TIN SACH SAU KHI CAP NHAT TRONG THU VIEN KAT=============================\n");
        printf("%-5d | %-25s | %-18s | %-10d | %-10s\n", 
            temp_book->id, temp_book->title, temp_book->author, temp_book->year, statusStr);
    }
}

// HÀM BỔ TRỢ: Tìm Node có giá trị ID nhỏ nhất (nằm ngoài cùng bên trái của cây/phân nhánh)
Tree findMin(Tree Root) {
    if (Root == NULL) return NULL;
    while (Root->left != NULL) {
        Root = Root->left;
    }
    return Root;
}

// HÀM: deleteBook
// Tham số: int id (mã sách cần xóa), Tree Root
// Mục đích: Xóa một node sách theo id ra khỏi Cây Tìm Kiếm Nhị Phân
// Đầu ra  : Trả về cây Root sau khi đã xóa node
Tree deleteBook(int id, Tree Root) {
    if (Root == NULL) {
        printf("Khong tim thay sach co ID %d de xoa!\n", id);
        return Root;
    }

    // 1. Tìm node cần xóa
    if (id < Root->data.id) {
        Root->left = deleteBook(id, Root->left);
    } 
    else if (id > Root->data.id) {
        Root->right = deleteBook(id, Root->right);
    } 
    else {
        // Đã tìm thấy node cần xóa (Root->data.id == id)

        // TRƯỜNG HỢP 1 & 2: Node lá hoặc Node chỉ có 1 con
        if (Root->left == NULL) {
            Tree temp = Root->right;
            free(Root);
            return temp;
        } 
        else if (Root->right == NULL) {
            Tree temp = Root->left;
            free(Root);
            return temp;
        }

        // TRƯỜNG HỢP 3: Node có 2 con
        // Tìm node nhỏ nhất bên nhánh phải để đưa lên thế chỗ
        Tree temp = findMin(Root->right);

        // Sao chép dữ liệu của node thế chỗ vào node hiện tại
        Root->data = temp->data;

        // Xóa node thế chỗ cũ ở nhánh phải
        Root->right = deleteBook(temp->data.id, Root->right);
    }

    return Root;
}

// Hàm đệ quy duyệt cây Trung Tự (Trái - Gốc - Phải)
void write_tree_to_file(Tree T, FILE* f) { // Sẽ giống hàm 
    if (T != NULL) {
        write_tree_to_file(T->left, f);
        fprintf(f, "%d,%s,%s,%d\n", T->data.id, T->data.title, T->data.author, T->data.year);
        write_tree_to_file(T->right, f);
    }
}

// Hàm phụ: Duyệt cây BST (In-Order) và ghi dữ liệu chuẩn định dạng CSV
void write_tree_to_csv(Tree root, FILE *f) {
    if (root != NULL) {
        write_tree_to_csv(root->left, f);

        // Ghi dữ liệu phân cách bằng dấu phẩy ","
        // Bọc Tên sách & Tác giả trong dấu "" để tránh lỗi nếu tên có chứa dấu phẩy
        fprintf(f, "%d,\"%s\",\"%s\",%d,%s\n",
                root->data.id,
                root->data.title,
                root->data.author,
                root->data.year,
                (root->data.status == AVAILABLE) ? "Co san" : "Dang muon");

        write_tree_to_csv(root->right, f);
    }
}

// Hàm chính: Xuất toàn bộ dữ liệu Cây BST ra file database.csv
void export_file(Tree root) {
    if (root == NULL) {
        printf("\n >> [THONG BAO]: Thu vien hien dang trong, khong co du lieu de xuat!\n");
        return;
    }

    FILE *f = fopen("database.csv", "w");
    if (f == NULL) {
        printf("\n >> [LOI]: Khong the tao hoac mo file database.csv!\n");
        return;
    }

    // Ghi hàng tiêu đề cột cho Excel
    fprintf(f, "ID,Ten Sach,Tac Gia,Nam Xuat Ban,Trang Thai\n");

    // Ghi toàn bộ dữ liệu sách
    write_tree_to_csv(root, f);

    fclose(f);
    printf("\n >> [THANH CONG]: Da xuat toan bo du lieu ra file 'database.csv'!\n");
    printf("    -> Ban co the tim file 'database.csv' trong thu muc project va mo bang Excel.\n");
}

// Hàm xử lý Mượn / Trả sách tự động
void borrow_return_book(Tree thu_vien) {
    if (thu_vien == NULL) {
        printf(" >> Thu vien hien dang trong!\n");
        return;
    }

    int id;
    printf(" Nhap Ma ID sach: ");
    if (scanf("%d", &id) != 1) {
        clear_buffer();
        printf(" >> [LOI]: Ma ID nhap vao khong hop le!\n");
        return;
    }
    clear_buffer();

    Tree bookNode = search_id(id, thu_vien);
    if (bookNode == NULL) {
        printf(" >> [LOI]: Khong tim thay sach co ID %d trong thu vien!\n", id);
        return;
    }

    // Hiển thị thông tin sách tìm thấy để xác nhận
    printf("\n --- THONG TIN SACH --- \n");
    printf(" Ten sach : %s\n", bookNode->data.title);
    printf(" Tac gia  : %s\n", bookNode->data.author);
    printf(" Trang thai: %s\n", (bookNode->data.status == AVAILABLE) ? "Co san trong thu vien" : "Dang duoc muon");
    printf(" ----------------------\n");
    printf(" Ban muon thuc hien thao tac gi?\n");
    printf(" 1. Muon sach\n");
    printf(" 2. Tra sach\n");
    printf(" Chon (1 hoac 2): ");

    int action;
    if (scanf("%d", &action) != 1) {
        clear_buffer();
        printf(" >> [LOI]: Lua chon khong hop le!\n");
        return;
    }
    clear_buffer();

    if (action == 1) { // Người dùng chọn MƯỢN SACH
        if (bookNode->data.status == BORROWED) {
            // Chặn không cho mượn nếu sách đã bị người khác mượn
            printf("\n >> [LOI]: Sach '%s' DANG DUOC MUON boi nguoi khac! Ban khong the muon luc nay.\n", bookNode->data.title);
        } else {
            bookNode->data.status = BORROWED;
            save_database(thu_vien);
            printf("\n >> [THANH CONG]: Ban da MUON thanh cong cuon sach '%s'!\n", bookNode->data.title);
        }
    } 
    else if (action == 2) { // Người dùng chọn TRẢ SÁCH
        if (bookNode->data.status == AVAILABLE) {
            // Chặn không cho trả nếu sách vốn đang nằm sẵn trong thư viện
            printf("\n >> [LOI]: Sach '%s' HIEN CO SAN trong thu vien (chua ai muon) nen khong the tra!\n", bookNode->data.title);
        } else {
            bookNode->data.status = AVAILABLE;
            save_database(thu_vien);
            printf("\n >> [THANH CONG]: Ban da TRA thanh cong cuon sach '%s'!\n", bookNode->data.title);
        }
    } 
    else {
        printf("\n >> [LOI]: Lua chon khong hop le! Vui long chon 1 hoac 2.\n");
    }
}
