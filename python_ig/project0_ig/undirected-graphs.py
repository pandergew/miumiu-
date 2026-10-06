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
            if v not in visited:
                stack.append(v)
                parent[v] = u 
            elif v != parent[u]:
                back_edges.append((parent[u], v))
    return tree_edges, back_edges

def bfs(graph, start):
    visited = {start}
    parent = {start: None}
    tree_edges = []
    cross_edges = []
    queue = deque([start])

    while queue:
        u = queue.popleft()
        for v in graph[u]:
            if v not in visited:
                stack.append(v)
                parent[v] = u 
            elif v != parent[u]:
                cross_edges.append((parent[u], v))
    return tree_edges, cross_edges
        

if __name__ == "__main__":
    graph = {
    'A': ['B', 'C'],
    'B': ['A', 'D'],
    'C': ['A', 'D'],
    'D': ['B', 'C'],
    }

    print(dfs(graph, 'A'))
    print(bfs(graph, 'A'))