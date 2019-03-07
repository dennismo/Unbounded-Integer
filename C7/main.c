//
//  main.c
//  C7
//
//  Created by Dennis Mo on 3/2/19.
//  Copyright © 2019 Dennis Mo. All rights reserved.
//

#include <stdio.h>

struct Node {
    int bigit;
    struct Node *next;
};

void print_num(struct Node *nlst);
struct Node *add(struct Node *n1lst, struct Node *n2lst);
struct Node *mult(struct Node *n1lst, struct Node *n2lst);

struct Node *reverse(struct Node *lst){
    if (lst == 0)
        return lst;
    struct Node *ptr = lst->next;
    if(ptr == 0)
        return lst;
    struct Node *ptrBefore = 0;
    struct Node *ptrAfter = lst;
    for (;lst->next ; lst = ptrAfter) {
        ptrAfter = lst->next;
        lst->next = ptrBefore;
        ptrBefore = lst;
    }
    lst->next = ptrBefore;
    return lst;
}

void print_num_help(struct Node *nlst){
    if (nlst){
        printf("%04d",nlst->bigit);
        print_num_help(nlst->next);
    }
}

void print_num(struct Node *nlst){
    if (nlst == 0) {
        printf("0");
    }
    else{
        struct Node *ptr = reverse(nlst);
        printf("%d",ptr->bigit);
        print_num_help(ptr->next);
        reverse(nlst);
    }
}

struct Node *cons_bigit(int bgt,struct Node *nxt){
    struct Node *ptr = malloc(sizeof(struct Node));
    ptr->bigit = bgt;
    ptr->next = nxt;
    return ptr;
}

/*
void free_num(struct Node *blst){
    struct Node *ptr = blst;
    struct Node *ptr2 = blst;
    while (ptr) {
        ptr = ptr->next;
        free(ptr2);
        ptr2 = ptr;
    }
}
*/

struct Node *copy_num(struct Node *nlst){
    struct Node *ptr = NULL;
    struct Node *copy = 0;
    struct Node *result = NULL;
    while (nlst) {
        if (copy == 0) {
            copy = cons_bigit(nlst->bigit,0);
            result = copy;
        }
        else{
            copy = cons_bigit(nlst->bigit,0);
            ptr->next = copy;
        }
        ptr = copy;
        nlst = nlst->next;
    }
    return result;
}

struct Node *add_inc(struct Node *nlst, int inc){
    int i = inc;
    struct Node *ptr = copy_num(nlst);
    struct Node *result = ptr;
    if (nlst == 0 && inc != 0) {
        return cons_bigit(inc, 0);
    }
    while (i) {
        if((i + ptr->bigit) > 9999){
            int x = ptr->bigit;
            ptr->bigit = i + x - 10000;
            i = (i + x) / 10000;
            if (ptr->next == 0) {
                ptr->next = cons_bigit(i,0);
                i = 0;
            }
            else
                ptr = ptr->next;
        }
        else{
            ptr->bigit = i + ptr -> bigit;
            i = 0;
        }
    }
    return result;
}

struct Node *add(struct Node *n1lst, struct Node *n2lst){
    int inc = 0;
    struct Node *ptr = 0;
    struct Node *cur = 0;
    struct Node *prePtr = 0;
    
    if (n1lst == 0) {
        return copy_num(n2lst);
    }
    if (n2lst == 0) {
        return copy_num(n1lst);
    }
    
    while(n1lst||n2lst){
        if (n1lst == 0) {
            cur = add_inc(n2lst, inc);
            inc = 0;
            prePtr->next = cur;
            break;
        }
        else if (n2lst == 0) {
            cur = add_inc(n1lst, inc);
            inc = 0;
            prePtr->next = cur;
            break;
        }
        else if (n1lst->bigit + n2lst->bigit + inc > 9999){
            cur = cons_bigit((n1lst->bigit + n2lst->bigit + inc) % 10000,0);
            inc = (n1lst->bigit + n2lst->bigit + inc) / 10000;
        }
        else{
            cur = cons_bigit(n1lst->bigit + n2lst->bigit + inc, 0);
            inc = 0;
        }
        if (prePtr == 0) {
            ptr = cur;
            prePtr = cur;
        }
        else {
            prePtr->next = cur;
            prePtr = cur;
        }
        n1lst = n1lst->next;
        n2lst = n2lst->next;
    }
    if (inc != 0) {
        cur->next = cons_bigit(inc, 0);
    }
    return ptr;
}
struct Node *mult_unit(struct Node *nlst, int num){
    int inc = 0;
    if (nlst == 0 || num == 0) {
        return 0;
    }
    struct Node *cur = NULL;
    struct Node *result = NULL;
    struct Node *prePtr = 0;
    while (nlst) {
        if (nlst->bigit * num + inc > 9999) {
            cur = cons_bigit((nlst->bigit * num + inc) % 10000, 0);
            inc = (nlst->bigit * num + inc) / 10000;
        }
        else{
            cur = cons_bigit((nlst->bigit * num + inc), 0);
            inc = 0;
        }
        if (prePtr == 0) {
            prePtr = cur;
            result = cur;
        }
        else{
            prePtr->next = cur;
            prePtr = cur;
        }
        nlst = nlst->next;
    }
    if (inc != 0) {
        cur->next = cons_bigit(inc, 0);
    }
    return result;
}

struct Node *mult(struct Node *n1lst, struct Node *n2lst){
    struct Node *cur = 0;
    struct Node *ptr = -1;
    int a = 1;
    
    if (!(n1lst && n2lst)) {
        return 0;
    }
    while (n2lst) {
        if (ptr == -1) {
            cur = mult_unit(n1lst, n2lst->bigit);
            ptr = cur;
        }
        else{
            struct Node *temp = mult_unit(n1lst, n2lst->bigit);
            if (temp == 0) {
                ++a;
                n2lst = n2lst->next;
                continue;
            }
            for (int i = 0; i < a; i++) {
                temp = cons_bigit(0,temp);
            }
            cur = add(ptr, temp);
//            free_num(ptr);
            //            free_num(temp);
            ptr = cur;
            ++a;
        }
        
        n2lst = n2lst->next;
    }
    
    return cur;
    
}

int main() {
    print_num(mult(
                   cons_bigit(1, cons_bigit(9998, cons_bigit(1, 0))),
                   cons_bigit(1, cons_bigit(9998, cons_bigit(1, 0)))
                  ));printf("\n");
    print_num(mult(cons_bigit(56, cons_bigit(1000,cons_bigit(20, cons_bigit(9010, 0)))), cons_bigit(0, cons_bigit(0, cons_bigit(10, 0)))));
    return 0;
}
