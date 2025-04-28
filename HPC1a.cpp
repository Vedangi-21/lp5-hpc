#include <iostream>
#include <stdlib.h>
#include <queue>
#include <omp.h>
using namespace std;

class node
{
public:
    node *left, *right;
    int data;
};

class Breadthfs
{
public:
    node *insert(node *, int);
    void bfs(node *);
};

node *Breadthfs::insert(node *root, int data)
{
    if (!root)
    {
        root = new node;
        root->left = NULL;
        root->right = NULL;
        root->data = data;
        return root;
    }

    queue<node *> q;
    q.push(root);

    while (!q.empty())
    {
        node *temp = q.front();
        q.pop();

        if (temp->left == NULL)
        {
            temp->left = new node;
            temp->left->left = NULL;
            temp->left->right = NULL;
            temp->left->data = data;
            return root;
        }
        else
        {
            q.push(temp->left);
        }

        if (temp->right == NULL)
        {
            temp->right = new node;
            temp->right->left = NULL;
            temp->right->right = NULL;
            temp->right->data = data;
            return root;
        }
        else
        {
            q.push(temp->right);
        }
    }
    return root;
}

void Breadthfs::bfs(node *head)
{
    if (!head) return;

    queue<node *> q;
    q.push(head);

    while (!q.empty())
    {
        int qSize = q.size();
        vector<node*> currentLevel;

        #pragma omp parallel for shared(q, currentLevel)
        for (int i = 0; i < qSize; i++)
        {
            node *currNode = NULL;

            #pragma omp critical
            {
                if (!q.empty())
                {
                    currNode = q.front();
                    q.pop();
                }
            }

            if (currNode)
            {
                #pragma omp critical
                cout << "\t" << currNode->data;

                #pragma omp critical
                {
                    if (currNode->left)
                        currentLevel.push_back(currNode->left);
                    if (currNode->right)
                        currentLevel.push_back(currNode->right);
                }
            }
        }

        for (auto child : currentLevel)
            q.push(child);
    }
}

int main()
{
    node *root = NULL;
    int data;
    char ans;
    Breadthfs b; 

    do
    {
        cout << "\nEnter data => ";
        cin >> data;
        root = b.insert(root, data);

        cout << "Do you want to insert one more node? (y/n) ";
        cin >> ans;

    } while (ans == 'y' || ans == 'Y');

    double start = omp_get_wtime();
    b.bfs(root); 
    double end = omp_get_wtime();

    cout << "\nExecution Time: " << (end - start) << " seconds\n";

    return 0;
}
