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
// 22/7 
// Khiet kiem du lieu dau vao kiểu .dat => Bắt buộc phải có hàm xuất file vì không đọc chay file gốc đc  tên: database.dat.
// Khiet làm 2 hàm xóa, tạo(book) để cho A bin thêm vô 
// Thanh làm tìm kiếm, load_database, save_database
// Bin viết ham xuất file, thêm Tree


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


// HÀM: readBook
// Tham số: Tree T
//        : FILE* f
// Mục đích: Ghi lại giá trị của từng Node Book được duyệt theo tiền tự của cây tránh việc khi sắp dữ liệu file lên cây trên Ram thành cây liên kết 
void readBook(Tree T, FILE* f) {
    if(T == NULL) return;
    else {
        fwrite(&T->data, sizeof(Book), 1, f);
        readBook(T->left, f);
        readBook(T->right, f);
    }
}

////Bin nghĩ hàm load là hàm lood của Thanh á nên để ở txt cho dễ đọc với sửa

// HÀM: lood_database
// Mục đích: Duyệt từ file để lấy từ Book làm nguồn cho hàm insertbook để trả về cây
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

// HÀM: save_database
// Tham số: Tree T
// Mục đích: Đem dữ liệu từ Tree T trả về cho file
// Đầu ra: Tree result sao khi dữ liệu trên file được đọc đưa lên cây
void save_database(Tree T) {
    FILE* f = fopen("database.dat", "wb");
    if(T == NULL) return;
    else {
        readBook(T, f);
    }
    fclose(f);
}


Tree search_id(int x, Tree Root) { //Khong thay tra ve NULL; Tra ve Tree // Minh chua hieu ham search
    if (Root == NULL) return NULL; // Truong hop tim khong thay x
    else {
        if(Root->data.id == x) {
            return Root;
        }
        else {
            if(x > Root->data.id) {
                return Search(x, Root->right);
            }
            else if (x < Root->data.id) {
                return Search(x, Root->left);
            }
        }
    }
}

// Hàm trả về trạng thái của sách. Chưa mượn trả về 1; Đã mượn trả về 0
int search_status(Tree T) {
    return T->data.status == AVAILABLE;
}

void search() {
    int x;
    printf("Vui lòng nhập id cuốn sách bạn tìm kiếm: ");
    scanf(" %d", &x);
    Tree result = lood_database();
    Tree test = search_id(x, result);
    if (test == NULL) {
        printf("Không tìm thấy cuốn sách có mã số ID");
    }
    else {
        if(search_status(test)) {
            printf("=================== DANH SACH SACH CO TRONG THU VIEN =============================\n");
            printf("%-5s | %-25s | %-18s | %-10s | %-10s\n", "ID", "Ten Sach", "Tac Gia", "Nam XB", "Trang Thai");
            printf("----------------------------------------------------------------------------------\n");
            printf("%-5d | %-25s | %-18s | %-10d | %-10s\n", test->data.id, test->data.author, test->data.year, "Trong kho sẵn sàng");
        }
        else {
            printf("=================== DANH SACH SACH CO TRONG THU VIEN =============================\n");
            printf("%-5s | %-25s | %-18s | %-10s | %-10s\n", "ID", "Ten Sach", "Tac Gia", "Nam XB", "Trang Thai");
            printf("----------------------------------------------------------------------------------\n");
            printf("%-5d | %-25s | %-18s | %-10d | %-10s\n", test->data.id, test->data.author, test->data.year, "Đang mượn");
        }
    }
}

// Hàm duyệt cây in ra màn hình 
void display_tree(Tree T) {
    if (T != NULL) {
        display_tree(T->left);
        char* statusStr = (T->data.status == AVAILABLE) ? "San sang" : "Dang muon";
        printf("%-5d | %-25s | %-18s | %-6d | %-10s\n", 
               T->data.id, T->data.title, T->data.author, T->data.year, statusStr);
        display_tree(T->right);
    }
}

// Hàm xóa bộ nhớ đệm (tránh lỗi trôi lệnh khi dùng scanf xong dùng fgets)
void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

Tree them_sach_moi(Tree thu_vien) {
    Book b;
    printf("\n--- NHAP THONG TIN SACH MOI ---\n");
    
    printf("Nhap ID (Ma sach): "); 
    scanf("%d", &b.id);
    clear_buffer(); 
    printf("Nhap ten sach: "); 
    fgets(b.title, 100, stdin); 
    b.title[strcspn(b.title, "\n")] = 0;
    printf("Nhap tac gia: "); 
    fgets(b.author, 50, stdin); 
    b.author[strcspn(b.author, "\n")] = 0;
    printf("Nhap nam xuat ban: "); 
    scanf("%d", &b.year);
    thu_vien = insertbook(b, thu_vien);
    printf(">> Da them sach '%s' vao he thong thanh cong!\n", b.title);
    return thu_vien;
}

// Hàm đệ quy duyệt cây Trung Tự (Trái - Gốc - Phải)
void write_tree_to_file(Tree T, FILE* f) {
    if (T != NULL) {
        write_tree_to_file(T->left, f);
        fprintf(f, "%d,%s,%s,%d\n", T->data.id, T->data.title, T->data.author, T->data.year);
        write_tree_to_file(T->right, f);
    }
}

// Hàm gọi chức năng xuất file
void export_file(Tree T) {
    FILE* f = fopen("DuLieuSach.txt", "w");
    if(f == NULL) {
        printf("Loi: Khong the tao hoac mo file DuLieuSach.txt!\n");
        return;
    }
    write_tree_to_file(T, f);
    fclose(f);
    printf(">> Da xuat/luu toan bo du lieu ra file DuLieuSach.txt!\n");
}

int main() {
    
}
