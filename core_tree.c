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
// Khiet kiem du lieu dau vao kiểu .dat => Bắt buộc phải có hàm xuất file vì không đọc chay file gốc đc  tên: database.dat
// Khiet làm 2 hàm xóa, tạo
// Thanh làm 2 hàm thêm, tìm kiếm
// Bin viết ham xuất file 



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

Tree sreachid(int x, Tree Root) {
    if (Root == NULL) return NULL;
    else {
        if(x.id > Root->data.id) {
            Search(x, Root->right);
        }
        else if((x.id < Root->data.id) ){
            Search(x, Root->left);
        }
        else{
            return;
        }
        else {
            printf("=================== DANH SACH SACH CO TRONG THU VIEN =============================\n");
            printf("%-5s | %-25s | %-18s | %-10s | %-10s\n", "ID", "Ten Sach", "Tac Gia", "Nam XB", "Trang Thai");
            printf("----------------------------------------------------------------------------------\n");
            printf("%-5d | %-25s | %-18s | %-10d | %-10s\n", test->data.id, test->data.author, test->data.year, "Đang mượn");
        }
    }
}


int main() {
    
}