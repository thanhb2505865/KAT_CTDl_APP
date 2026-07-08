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
typedef struct TreeNode {
    Book data;              
    struct TreeNode* left;  
    struct TreeNode* right; 
} TreeNode;// 

// 22/7 
// Khiet kiem du lieu dau vao
// Bin tùm cach đọc file 
// Thanh biên dịch file C
// Khiet làm 2 hàm xóa, tạo
// Thanh làm 2 hàm thêm, tìm kiếm
// Bin viết ham xuất file 
