#include "Headers/oledfont.h"
#include "Headers/note.h"
#include <REGX52.H>
#include "Headers/oled.h"


typedef struct NoteNode {
	unsigned char x;      // 当前X坐标
	unsigned char y;      // Y坐标(轨道)
	unsigned char speed;  // 移动速度
	struct NoteNode* next; // 指向下一个音符节点
} NoteNode;

NoteNode* CreateNoteNode(unsigned char track, unsigned char speed) {
    NoteNode* newNote = (NoteNode*)malloc(sizeof(NoteNode));
    if (newNote == NULL) {
        // 内存不足处理
        return NULL;
    }

    newNote->x = 119;  // 从右侧开始
    newNote->y = track * 3 ;  // 轨道1=1, 轨道2=4
    newNote->speed = speed;
    newNote->next = NULL;
    return newNote;
}

void AddNoteToHead(NoteNode** head, NoteNode* newNote) {
    if (newNote == NULL) return;

    newNote->next = *head;
    *head = newNote;
}

void RemoveNote(NoteNode** head, NoteNode* noteToRemove) {
    if (*head == NULL || noteToRemove == NULL) return;

    // 如果要删除的是头节点
    if (*head == noteToRemove) {
        *head = noteToRemove->next;
        free(noteToRemove);
        return;
    }

    // 查找要删除的节点
    NoteNode* current = *head;
    while (current->next != NULL && current->next != noteToRemove) {
        current = current->next;
    }

    // 如果找到节点，删除它
    if (current->next == noteToRemove) {
        current->next = noteToRemove->next;
        free(noteToRemove);
    }
}