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
// Khiet kiem du lieu dau vao
// Bin tùm cach đọc file 
// Thanh biên dịch file C
// Khiet làm 2 hàm xóa, tạo
// Thanh làm 2 hàm thêm, tìm kiếm
// Bin viết ham xuất file 

void InsertBook(Book x, Tree* Root) { //Tree* = struct Node**
    if(*Root == NULL) {
        Tree New = (Tree)malloc(sizeof(struct Node));
        x.status = AVAILABLE;
        New->data = x;
        New->left = NULL;
        New->right = NULL;
        *Root = New;
    }
    else {
        if(x.id > (*Root)->data.id) {
            InsertBook(x, &(*Root)->right);
        }
        else if((x.id < (*Root)->data.id) ){
            InsertBook(x, &(*Root)->left);
        }
        else{
            return;
        }
    }
}

Tree Search(Book x, Tree Root) { //Khong thay tra ve NULL; Tra ve Tree // Minh chua hieu ham search
    if (Root == NULL) return NULL; // Truong hop tim khong thay x
    else {
        if(x.id > Root->data.id) {
            return Search(x, Root->right);
        }
        else if((x.id < Root->data.id) ){
            return Search(x, Root->left);
        }
        else{
            return;
        }
    }
}

// Lamf chuaw

