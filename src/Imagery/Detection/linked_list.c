#include "Imagery/Detection/linked_list.h"

// Initialization
Node *initializeNode(void *value){
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
        errx(1, "Initialize Node: not enough memory");
    newNode->value = value;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

void initializeMyList(MyList *list, size_t nb, void *value){
    if (nb){
        list->head = initializeNode(value);
        list->tail = list->head;
        list->length = 1;
        for(size_t i = 1; i < nb; i++){
            append(list, value);
        }
    }
    else{
        list->head = NULL;
        list->tail = NULL;
        list->length = 0;
    }
}

// Getting nodes and their values
Node *get_node(MyList *list, size_t index){
    if(index >= list->length)
        return NULL;
    Node *node = list->head;
    for(;index > 0;index--){
        node = node->next;
    }
    return node;
}

void *get_value(MyList *list, size_t index){
    Node *tmp = get_node(list, index);
    if (tmp == NULL){
        return NULL;
    }
    return tmp->value;
}

// Insertion of node
void append(MyList *list, void *value){
    Node *node = initializeNode(value);
    if (list->head == NULL){
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }
    list->length++;
}
void prepend(MyList *list, void *value){
    Node *node = initializeNode(value);
    if (list->head == NULL){
        list->head = node;
        list->tail = node;
    } else {
        list->head->prev = node;
        node->next = list->head;
        list->head = node;
    }
    list->length++;
}

void insert(MyList *list, size_t index, void *value){
    if (index == 0){
        prepend(list, value);
    } else if (index == list->length){
        append(list, value);
    } else {
        Node *node = initializeNode(value);
        Node *tmp = get_node(list, index);
        tmp->prev->next = node;
        node->prev = tmp->prev;
        node->next = tmp;
        tmp->prev = node;
        list->length++;
    }
}

// Removing node
void *prepop(MyList *list){
    if (list->head == NULL){
        return NULL;
    }
    void * value = list->head->value;
    list->head = list->head->next;
    if (list->head != NULL){
        list->head->prev = NULL;
    }
    list->length--;
    return value;
}

void *pop(MyList *list){
    if (list->tail == NULL){
        return NULL;
    }
    void * value = list->tail->value;
    list->tail = list->tail->prev;
    list->tail->next = NULL;
    list->length--;
    return value;
}

void *removeAt(MyList *list, size_t index){
    if (index >= list->length){
        return NULL;
    }
    Node *tmp = get_node(list, index);
    if (tmp == list->head){
        return prepop(list);
    } else if (tmp == list->tail){
        return pop(list);
    } else {
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
        void * value = tmp->value;
        free(tmp);
        list->length--;
        return value;
    }
}

// Freeing memory
// Free the memory of the node
void free_node(Node *node){
    free(node);
}

void free_list(MyList *list){
    Node *tmp = list->head;
    while(tmp != NULL){
        Node *next = tmp->next;
        free_node(tmp);
        tmp = next;
    }
}

// Display Node
void printNode(Node node){
    printf("Node: %p\n", node.value);
}

// Display List
void printMyList(MyList *list){
    Node *tmp = list->head;
    printf("[");
    for(;tmp->next != NULL;tmp = tmp->next){
        printf("  %d ;", *((int *)tmp->value));
    }
    printf("  %d ]\n", *((int *)tmp->value));
}   