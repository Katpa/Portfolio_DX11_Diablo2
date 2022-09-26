#include "Framework.h"

AStarPlus::AStarPlus(GameTileMap* map)
    :AStar()
{
    map->GetNodes(nodes);
    SetEdge(map->GetWidth());

    heap = new Heap();
}

AStarPlus::~AStarPlus()
{
}

void AStarPlus::SetEdge(UINT width)
{
    for (UINT i = 0; i < nodes.size(); i++)
    {
        UINT remainder = i % width;

        //가로줄 오른쪽 타일과 연결
        if (remainder != width - 1) //가로줄의 맨끝이 아닌경우
        {
            nodes[i]->AddEdge(nodes[i + 1]);
            nodes[i + 1]->AddEdge(nodes[i]);

            //상상우
            if (i < nodes.size() - (width * 2))
            {
                int tmp = nodes.size();
                if (nodes[i + width]->ReturnState() != Node::OBSTACLE &&
                    nodes[i + width + 1]->ReturnState() != Node::OBSTACLE)
                {
                    nodes[i]->AddEdge(nodes[i + (width * 2) + 1]);
                    nodes[i + (width * 2) + 1]->AddEdge(nodes[i]);
                }
            }
        }

        //세로줄 맨 위에 있는 가로줄 제외하고 연결
        if (i < nodes.size() - width)
        {
            nodes[i]->AddEdge(nodes[i + width]);
            nodes[i + width]->AddEdge(nodes[i]);

            //우우상
            if (remainder < width - 2)
            {
                if (nodes[i + 1]->ReturnState() != Node::OBSTACLE &&
                    nodes[i + width + 1]->ReturnState() != Node::OBSTACLE)
                {
                    nodes[i]->AddEdge(nodes[i + width + 2]);
                    nodes[i + width + 2]->AddEdge(nodes[i]);
                }
            }

            //좌좌상
            if (remainder > 1)
            {
                if (nodes[i - 1]->ReturnState() != Node::OBSTACLE &&
                    nodes[i + width - 1]->ReturnState() != Node::OBSTACLE)
                {
                    nodes[i]->AddEdge(nodes[i + width - 2]);
                    nodes[i + width - 2]->AddEdge(nodes[i]);
                }
            }
        }

        //한칸 대각선
        if (i < nodes.size() - width)
        {
            if (remainder != width - 1)
            {
                nodes[i]->AddEdge(nodes[i + width + 1]);
                nodes[i + width + 1]->AddEdge(nodes[i]);
            }

            if (remainder != 0)
            {
                nodes[i]->AddEdge(nodes[i + width - 1]);
                nodes[i + width - 1]->AddEdge(nodes[i]);
            }
        }

        //상상좌
        if (i < nodes.size() - (width * 2) && remainder > 0)
        {
            if (nodes[i + width]->ReturnState() != Node::OBSTACLE &&
                nodes[i + width - 1]->ReturnState() != Node::OBSTACLE)
            {
                nodes[i]->AddEdge(nodes[i + (width * 2) - 1]);
                nodes[i + (width * 2) - 1]->AddEdge(nodes[i]);
            }
        }
    }
}
