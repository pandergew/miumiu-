from collections import deque

def dfs(graph, start):
    """ Depth first search με στοίβα (LIFO) σε μη κατευθυνόμενο γράφο 
    και επίσης κάνει return(tree_edges, back_edges)"""
    visited = set()         #κορυφές που έχουν επισκεφθεί 
    parent = {start: None}  #parent[v] = η κορυφή από την οποία βρέθηκε το v
    tree_edges = []         #ακμές δεντρου 
    back_edges = []         #ακμές οπισθοχώρησης 
    stack = [start]         #η στοίβα ξεκινά με την αρχική κορυφή 

    while stack:
        u = stack.pop()     #βγάζουμε την τελευταία κορυφή που μπήκε
        if u in visited:    #η κορυφή μπορεί να μπήκε πολλές φορές στην στοίβα
            continue
        visited.add(u)      #στο DFS σημειώνουμε την επίσκεψη όταν βγαίνει από τη στοίβα
        
        #η ακμή από τον τελικό πατέρα προς το u είναι ακμή δένδρου 
        if parent[u] is not None:
            tree_edges.append((parent[u], u)) 

        for v in graph[u]:
            if v not in visited:
                #αν δεν υπάρχει το v στο visited τη βάζουμε στην στοίβα αν μπει ξανά από άλλη 
                #κορυφή, ο parent ενημερώνεται και μετράει ο τελευταίος (αυτός που θα την επισκευθεί) 
                stack.append(v)
                parent[v] = u 
            elif v != parent[u]:
                #αν είναι ήδη επισκεμμένη κορυφή που δεν είναι ο πατέρας του u τότε δήνει ακμή οπισθοχόρησης 
                #την καταγράφουμε μόνο από την πλευρά του απογόνου
                back_edges.append((u, v))

    return tree_edges, back_edges



def bfs(graph, start):
    """ Breadth first search με ουρά (FIFO) σε μη κατευθυνόμενο γράφο
        και επίσης κάνει return (tree_edges, cross_edges)"""
    visited = {start}       #στο BFS σημειώνουμε την επίσκεψη μόλις μπει στην ουρά 
    parent = {start: None}
    tree_edges = []         #ακμές δέντρου
    cross_edges = []        #εγκαρσιες ακμές
    queue = deque([start])

    while queue:
        u = queue.popleft() #βγάζουμε την πρώτη κορυφή που μπήκε 
        for v in graph[u]:
            if v not in visited:
                #αν το v δεν είναι στο visited, το σημειώνουμε αμέσως ως επισκεμμένο
                #και το βάζουμε στην ουρά, ώστε να μπει μία μόνο φορά,
                #η ακμή (u, v) που το ανακάλυψε είναι ακμή δέντρου
                visited.add(v)
                queue.append(v)
                parent[v] = u 
                tree_edges.append((u, v))   #η ακμή που ανακάλυψε το v
            elif v != parent[u] and (v, u) not in cross_edges:
                #ήδη επισκεμμένη κορυφή που δεν είναι ο πατέρας του u: εγκάρσια ακμή.
                #ο έλεγχος (v, u) αποφεύγει τη διπλή καταγραφή της ίδιας ακμής
                #από τις δύο πλευρές
                cross_edges.append((u, v))
    return tree_edges, cross_edges
        

if __name__ == "__main__":
    #παράδειγμα γγράφου(τετράγωνο) με λίστες γειτνίασης
    graph = {
    'A': ['B', 'C'],
    'B': ['A', 'D'],
    'C': ['A', 'D'],
    'D': ['B', 'C'],
    }

    print(dfs(graph, 'A'))
    print(bfs(graph, 'A'))