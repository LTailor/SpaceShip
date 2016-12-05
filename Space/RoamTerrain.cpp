#include "RoamTerrain.h"

namespace Roam
{
	RoamTerrain::RoamTerrain()
	{
	}


	RoamTerrain::~RoamTerrain()
	{
	}

	void RoamTerrain::initialize(const float* data, int length)
	{
		if (length < 3) return;

		this->firstNode = make_shared<PolygonNode>();
		shared_ptr<PolygonNode> current = this->firstNode;
		this->polygonCount = length / 9;

		for (int i = 0; i < length / 9; i++)
		{
			current->parent = nullptr;
			current->pointers[0] = glm::vec3(data[i * 9], data[i * 9 + 1], data[i * 9 + 2]);
			current->pointers[1] = glm::vec3(data[i * 9 + 3], data[i * 9 + 4], data[i * 9 + 5]);
			current->pointers[2] = glm::vec3(data[i * 9 + 6], data[i * 9 + 7], data[i * 9 + 8]);

			if (i < length / 9 - 1)
			{
				current->next = make_shared<PolygonNode>();
				current->next->prev = current;
				current = current->next;
			}

			lastNode = current;
		}
	}

	void RoamTerrain::addRenderNode(shared_ptr<PolygonNode> node)
	{
		lastNode->next = node;
		node->prev = lastNode;
		lastNode = node;
	}

	void RoamTerrain::splitPolygon(shared_ptr<PolygonNode> node)
	{
		vec3 fs = node->pointers[1] - node->pointers[0];
		fs /= 2;
		fs += node->pointers[0];

		vec3 st = node->pointers[2] - node->pointers[1];
		st /= 2;
		st += node->pointers[1];

		vec3 ft = node->pointers[0] - node->pointers[2];
		ft /= 2;
		ft += node->pointers[0];

		node->firstChild = make_shared<PolygonNode>(node);
		node->firstChild->pointers[0] = node->pointers[0];
		node->firstChild->pointers[1] = fs;
		node->firstChild->pointers[2] = ft;

		node->secondChild = make_shared<PolygonNode>(node);
		node->secondChild->pointers[0] = fs;
		node->secondChild->pointers[1] = node->pointers[1];
		node->secondChild->pointers[2] = st;

		node->thirdChild = make_shared<PolygonNode>(node);
		node->thirdChild->pointers[0] = ft;
		node->thirdChild->pointers[1] = st;
		node->thirdChild->pointers[2] = node->pointers[2];

		node->centerChild = make_shared<PolygonNode>(node);
		node->centerChild->pointers[0] = st;
		node->centerChild->pointers[1] = ft;
		node->centerChild->pointers[2] = fs;

		addRenderNode(node->firstChild);
		addRenderNode(node->secondChild);
		addRenderNode(node->thirdChild);
		addRenderNode(node->centerChild);
	}

	void RoamTerrain::reducePolygon(PolygonNode* node)
	{

	}

	int RoamTerrain::getAllVertices(vec3** result) const
	{
		int vertexCount = polygonCount * 3;
		vec3* data = new vec3[vertexCount];

		shared_ptr<PolygonNode> current = firstNode;
		int index = 0;
		while (current != nullptr)
		{
			data[index] = current->pointers[0];
			data[index + 1] = current->pointers[1];
			data[index + 2] = current->pointers[2];
			current = current->next;
			index += 3;
		}
		*result = data;

		return vertexCount;
	}
}