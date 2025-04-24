#include "Headers/oledfont.h"
#include "Headers/note.h"
#include <REGX52.H>
#include "Headers/oled.h"


typedef struct NoteNode {
	unsigned char x;      // ��ǰX����
	unsigned char y;      // Y����(���)
	unsigned char speed;  // �ƶ��ٶ�
	struct NoteNode* next; // ָ����һ�������ڵ�
} NoteNode;

NoteNode* CreateNoteNode(unsigned char track, unsigned char speed) {
    NoteNode* newNote = (NoteNode*)malloc(sizeof(NoteNode));
    if (newNote == NULL) {
        // �ڴ治�㴦��
        return NULL;
    }

    newNote->x = 119;  // ���Ҳ࿪ʼ
    newNote->y = track * 3 ;  // ���1=1, ���2=4
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

    // ���Ҫɾ������ͷ�ڵ�
    if (*head == noteToRemove) {
        *head = noteToRemove->next;
        free(noteToRemove);
        return;
    }

    // ����Ҫɾ���Ľڵ�
    NoteNode* current = *head;
    while (current->next != NULL && current->next != noteToRemove) {
        current = current->next;
    }

    // ����ҵ��ڵ㣬ɾ����
    if (current->next == noteToRemove) {
        current->next = noteToRemove->next;
        free(noteToRemove);
    }
}