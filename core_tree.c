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
    }
    else {
        if(x.id > Root->data.id) {
            Root->right = insertbook(x, Root->right);
        }
        else if((x.id < Root->data.id) ){
            Root->left = insertbook(x, Root->left);
        }
        else{
            return Root;
        }
    }
}

// HÀM: lood_database
// Mục đích: Đem dữ liệu từ file dựng thành cây trên Ram
// Đầu ra: Tree result sao khi dữ liệu trên file được đọc đưa lên cây
Tree lood_database() {
    Tree result = NULL;
    FILE* f = fopen("database.dat", "rb");
    Book temp;
    while(fread(&temp, sizeof(Book), 1, f) == 1) {
        result = insertbook(temp, result);
    }
    fclose(f);
    return result;
}

// Hàm duyệt cây in ra màn hình 
void display_tree(Tree thu_vien) {
    if (thu_vien != NULL) {
        display_tree(thu_vien->left);
        char* statusStr = (thu_vien->data.status == AVAILABLE) ? "Trong kho sẵn sàng" : "Dang muon";
        printf("=================== DANH SACH SACH CO TRONG THU VIEN KAT=============================\n");
        printf("%-5d | %-25s | %-18s | %-10d | %-10s\n", 
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
    temp->status = AVAILABLE;
}

// Ham print Book
void printbook(Book virtual) {
    char* statusStr = (virtual.status == AVAILABLE) ? "Trong kho sẵn sàng" : "Dang muon";
    printf("=================== THONG TIN SACH TRONG THU VIEN KAT=============================\n");
    printf("%-5d | %-25s | %-18s | %-10d | %-10s\n", 
        virtual.id, virtual.title, virtual.author, virtual.year, statusStr);
}

// HÀM: insertnode
// Tham số: Book x, Tree Root
// Mục đích: 
// Đầu ra:
Tree insertnode(Tree thu_vien) { // Thieu ne a bin phai kiem truoc khi them 
    Book b;
    printf("\n--- NHAP THONG TIN SACH MOI ---\n");
    scanfbook(&b);
    thu_vien = insertbook(b, thu_vien);
    printf(">> Da them sach '%s' vao he thong thanh cong!\n", b.title);
    return thu_vien;
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

// Hàm trả về trạng thái của sách. Chưa mượn trả về 1; Đã mượn trả về 0
int search_status(Tree T) {
    return T->data.status == AVAILABLE;
}

// HÀM: search
// Mục đích: Tìm và in ra Book 
void search() {
    int x;
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
            printf("%-5s | %-25s | %-18s | %-10s | %-10s\n", "ID", "Ten Sach", "Tac Gia", "Nam XB", "Trang Thai");
            printf("----------------------------------------------------------------------------------\n");
            printf("%-5d | %-25s | %-18s | %-10d | %-10s\n", test->data.id, test->data.author, test->data.year, "Trong kho sẵn sàng");
        }
        else {
            printf("=================== DANH SACH SACH CO TRONG THU VIEN KAT=============================\n");
            printf("%-5s | %-25s | %-18s | %-10s | %-10s\n", "ID", "Ten Sach", "Tac Gia", "Nam XB", "Trang Thai");
            printf("----------------------------------------------------------------------------------\n");
            printf("%-5d | %-25s | %-18s | %-10d | %-10s\n", test->data.id, test->data.author, test->data.year, "Đang mượn");
        }
    }
}

// HÀM: change_inf_book
// Tham số: Tree thu_vien
// Mục đích: Thay doi cac thong tin cua Book trong Thu vien
void change_inf_book(Tree thu_vien) {
    Book virtual;
    printf("Nhap ID (Ma sach) ban muon thay doi: ");
    int temp;
    scanf("%d", &temp);
    Tree search = search_id(temp, thu_vien);
    if (search == NULL) {
        printf("Không tìm thấy cuốn sách có mã số ID\n");
    }
    else {
        scanfbook(&virtual);
        thu_vien = insertbook(virtual, thu_vien);
        printf("Cap nhat du lieu sach thanh cong\n");
        char* statusStr = (virtual.status == AVAILABLE) ? "Trong kho sẵn sàng" : "Dang muon";
        printf("=================== THONG TIN SACH SAU KHI CAP NHAT TRONG THU VIEN KAT=============================\n");
        printf("%-5d | %-25s | %-18s | %-10d | %-10s\n", 
            virtual.id, virtual.title, virtual.author, virtual.year, statusStr);
    }
    save_database(thu_vien);
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

// Hàm gọi chức năng xuất file
void export_file(Tree T) {
    FILE* f = fopen("database.csv", "w");
    if(f == NULL) {
        printf("Loi: Khong the tao hoac mo file database.csv!\n");
        return;
    }
    write_tree_to_file(T, f);
    fclose(f);
    printf(">> Da xuat/luu toan bo du lieu ra file database.csv!\n");
}
