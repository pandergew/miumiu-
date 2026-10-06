from collections import deque

def dfs(graph, start):
    visited = set()
    parent = {start: None}
    tree_edges = []
    back_edges = []
    stack = [start]

    while stack:
        u = stack.pop()
        if u in visited:
            continue
        visited.add(u) 
        if parent[u] is not None:
            tree_edges.append((parent[u], u)) 
        for v in graph[u]:
            if v is not visited:
                visited.add(v)
                parent[v] = u 
            elif v != parent[u]:
                back_edges.append((parent[u], v))
            pass
    return tree_edges, back_edges

def bfs(graph, start):


if name == "main":
    graph = {
    'A': ['B', 'C'],
    'B': ['A', 'D'],
    'C': ['A', 'D'],
    'D': ['B', 'C'],
    }

    print(dfs(graph, 'A'))
    print(bfs(graph, 'A'))