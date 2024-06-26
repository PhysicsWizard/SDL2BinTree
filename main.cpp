#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "src/inc/SDL.h"
#include "src/inc/SDL_ttf.h"

#undef main
using namespace std;

//Nodes hold reference to their left and right child, their parent and the spacing they need to be placed.
struct Node {
    int data;
    int horizontalSpacing = 10;
    Node *left;
    Node *right;
    Node *parent;
    SDL_Rect nodeRect;
};

class BinaryTree {
    Node *root;
    TTF_Font *font;
    SDL_Renderer *renderer;
    int totalNodes = 0;
    int levelSpacing = 50;
    int totalDepth = 0;

//Tree Class requires SDl Renderer and TTF font to be created (For visulaisation)
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
    int GetSubTreeWidth(Node* );
    void SetNodePositions(Node*, int, int);
};


//Recursively gets the total width of a node's subtree.
int BinaryTree::GetSubTreeWidth(Node* p){
    if(p == nullptr)return 0;

    int leftWidth = GetSubTreeWidth(p->left);
    int rightWidth = GetSubTreeWidth(p->right);

    return leftWidth + rightWidth + 1;
}

//Using the nodes subtree width, creates an x offset to give the node's children. Recursively Sets the positions of each node from p. 
void BinaryTree::SetNodePositions(Node*p, int x, int y){
    if (p == nullptr) return;
    int subTreeWidth = GetSubTreeWidth(p);
    int offset = subTreeWidth * 10;


    p->nodeRect.x = x;
    p->nodeRect.y = y;

    if (p->left != nullptr)
    {
        SetNodePositions(p->left, x - offset, y + levelSpacing);
    }

    if (p->right != nullptr)
    {
        SetNodePositions(p->right, x + offset, y + levelSpacing);
    }
    
}

void BinaryTree::RenderNode(SDL_Renderer *renderer, Node *node, Node *parent){
    
    //Make data string, draw node in red; set render draw to white.
    string dataString = to_string(node->data);
    SDL_SetRenderDrawColor(renderer, 255,50,0,255);
    SDL_RenderDrawRect(renderer, &node->nodeRect);
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    
    //Connect Node to parent
    if (node->parent != nullptr)
    {
        SDL_RenderDrawLine(renderer, node->nodeRect.x + node->nodeRect.w/2, node->nodeRect.y, parent->nodeRect.x + parent->nodeRect.w/2, parent->nodeRect.y + parent->nodeRect.h);
    }

    //Draw text
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, dataString.c_str(), {255,255,255});
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    //Free Memory
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(renderer, textTexture, NULL,&node->nodeRect);
    SDL_DestroyTexture(textTexture);
}

void BinaryTree::insert(int item) {
    //Setup inputed node.
    Node *p = new Node;
    Node *parent;
    p->data=item;
    p->left = nullptr;
    p->right = nullptr;
    p->parent = nullptr;
    p->nodeRect.w = 30;
    p->nodeRect.h = 30;

    if (isEmpty()){
        root = p;
        }


    else {
        Node *ptr;
        ptr = root;

        //Traverse Tree until reaching the bottom; set the correct references for parent node.
        while (ptr!=nullptr) {
            parent = ptr;
            if (item < ptr->data){
                ptr = ptr->left;
            }
            else
                ptr = ptr->right;
        }
        if (item < parent->data){
            parent->left=p;
            p->parent = parent;
            }
        else{
            parent->right=p;
            p->parent = parent;
        }
    }
}

void BinaryTree::displayTree() {
    root->horizontalSpacing = 0;
    int startX = 640;
    int startY = 100;
    SetNodePositions(root, startX,startY);
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

void AddRandomNodes(BinaryTree binTree, int desiredNodes){
    for(int i = 0; i <= desiredNodes; i++){
        int randomInt = rand() % 100;
        binTree.insert(randomInt);
    }
}

int main(int argc, char** args){
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    //Setup SDL
    if (IntizializeSDL() != 0)
    {
       cout << "Error Initializing SDL" << endl;
        system("pasuse");
       return 1;
    }
    
    window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1280,720, SDL_WINDOW_SHOWN);

    if(!window){
        cout << "Error Creating Window" << SDL_GetError() << endl;
        system("pause");
        return 1;
    }

    SDL_SetWindowResizable(window, SDL_TRUE);

    renderer = SDL_CreateRenderer(window,0,0);

    if(!renderer){
        cout << "Error Creating Renderer" << SDL_GetError() << endl;
        system("pause");
        return 1;
    }
    
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0,100,255,255);
    //Import ttf Font
    TTF_Font *font = TTF_OpenFont("src/SmallTypeWriting.ttf", 1028);
    if (font == nullptr)
    {
        cout << "Could not load font" << endl;
        return 1;
    }

    BinaryTree tree = BinaryTree(font,renderer);
    // tree.insert(50);
    tree.insert(10);
    AddRandomNodes(tree, 50);

    tree.displayTree();

    SDL_RenderPresent(renderer);

    SDL_Event e;
    EventLoop(e, window,renderer,font);
}
