#include <iostream>
#include <vector>
#include <string>
#include "src/inc/SDL.h"
#include "src/inc/SDL_ttf.h"

#undef main
using namespace std;

struct Node {
    int data;
    Node *left;
    Node *right;
    SDL_Rect nodeRect;
};

class BinaryTree {
    Node *root;
    TTF_Font *font;
    SDL_Renderer *renderer;
public:
    BinaryTree(TTF_Font *font,SDL_Renderer *renderer){
        root = nullptr;
        this->font = font;
        this->renderer = renderer;
    }
    int isEmpty() {
        return (root == nullptr);
    }
    void insert(int item    );
    void displayTree();
    void printTree(Node *, Node *);
    void RenderNode(SDL_Renderer *renderer, Node *node, Node *parent);
};

void BinaryTree::RenderNode(SDL_Renderer *renderer, Node *node, Node *parent){
    if(parent == nullptr){
        node->nodeRect.x = ((640 - node->nodeRect.w)/2);
        node->nodeRect.y = ((480 - node->nodeRect.h)/2) - 200;
    }
    else{
        node->nodeRect.y = parent->nodeRect.y + 50;
        
        int newXvalue = parent->nodeRect.x;
        if (node->data > parent->data)
        {
            newXvalue += 50;
            cout<<"Larger Value" << endl;
        }
        else{
            newXvalue -= 50;
            cout<<"Smaller Value" << endl;
        }
        
        
        
        node->nodeRect.x = newXvalue;
    }


    string dataString = to_string(node->data);
    SDL_SetRenderDrawColor(renderer, 255,50,0,255);
    SDL_RenderDrawRect(renderer, &node->nodeRect);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, dataString.c_str(), {255,255,255});
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL,&node->nodeRect);
    SDL_DestroyTexture(textTexture);
    cout << "Done" << endl;
}

void BinaryTree::insert(int item) {
    Node *p = new Node;
    Node *parent;
    p->data=item;
    p->left = nullptr;
    p->right = nullptr;
    p->nodeRect.w = 30;
    p->nodeRect.h = 30;

    if (isEmpty()){root = p;}
    else {
        Node *ptr;
        ptr = root;
        while (ptr!=nullptr) {
            parent = ptr;
            if (item < ptr->data)
                ptr = ptr->left;
            else
                ptr = ptr->right;
        }
        if (item < parent->data)
            parent->left=p;
        else
            parent->right=p;
    }
}

void BinaryTree::displayTree() {
    printTree(root, nullptr);
}

void BinaryTree::printTree(Node *ptr, Node *parent) {
    if (ptr!=nullptr) {
        printTree(ptr->right, ptr);
        RenderNode(renderer, ptr, parent);
        printTree(ptr->left, ptr);
    }
}
int IntizializeSDL(){
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
        return 1;
    }

    if(TTF_Init() < 0){
        return 1;
    }

    return 0;
}

void EventLoop(SDL_Event e, SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font){
    bool quit = false;

    while(!quit){
        while (SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT){
                quit = true;
            }
        }  
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font);
    SDL_Quit;
}
int main(int argc, char** args){
    cout << "Hello" << endl;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (IntizializeSDL() != 0)
    {
       cout << "Error Initializing SDL" << endl;
        system("pasuse");
       return 1;
    }
    
    window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,480, SDL_WINDOW_SHOWN);

    if(!window){
        cout << "Error Creating Window" << SDL_GetError() << endl;
        system("pause");
        return 1;
    }

    renderer = SDL_CreateRenderer(window,0,0);

    if(!renderer){
        cout << "Error Creating Renderer" << SDL_GetError() << endl;
        system("pause");
        return 1;
    }
    
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0,100,255,255);
    
    TTF_Font *font = TTF_OpenFont("src/SmallTypeWriting.ttf", 256);
    if (font == nullptr)
    {
        cout << "Could not load font" << endl;
        return 1;
    }

    BinaryTree tree = BinaryTree(font,renderer);
    tree.insert(10);
    tree.insert(8);
    tree.insert(76);
    tree.insert(15);
    tree.insert(2);
    tree.insert(0);
    tree.insert(99);
    tree.insert(54);
    tree.insert(16);
    tree.insert(33);
    tree.insert(89);
    tree.displayTree();

    SDL_RenderPresent(renderer);

    SDL_Event e;
    EventLoop(e, window,renderer,font);

    
}
