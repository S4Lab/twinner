#!/usr/bin/env python3
import networkx as nx
from networkx.drawing.nx_pydot import read_dot
from networkx.algorithms.similarity import graph_edit_distance
from networkx.algorithms.similarity import optimize_graph_edit_distance
import sys
import re


def main():
    print("Graph Similarity Measure Calculator")
    if len(sys.argv) != 3:
        print("Usage: calc-sim-measure.py column-dot-files.txt row-dot-files.txt")
        return
    clist, rlist = load_graphs(sys.argv[1], sys.argv[2])
    if len(clist) != len(rlist):
        print("Number of rows and columns do not match")
        return
    print("%d graphs are loaded." % (len(clist)*2))
    print("Filling the {0}x{0} similarity matrix...".format(len(clist)))
    sim_matrix = {}
    for cname, G1 in clist:
        for rname, G2 in rlist:
            print("\tComparing %s with %s" % (rname, cname))
            sm = approx_sim_measure(G1, G2, 12)
            print("\t\tFinal sim measure: %f" % sm)
            sim_matrix[(rname, cname)] = sm
    print_matrix(sim_matrix,
            [name for name, G in rlist],
            [name for name, G in clist])

def print_matrix(mat, rows, cols):
    print("after/before & " + " & ".join(cols) + r"\\")
    for r in rows:
        print(r + " & ")
        print("\t" +
                " & ".join([str(mat[(r, c)]) for c in cols]) +
                r"\\")

def approx_sim_measure(G1, G2, timeout):
    import signal
    class TimeoutError(Exception):
        pass
    def handler(signum, frame):
        raise TimeoutError()
    signal.signal(signal.SIGALRM, handler)
    signal.alarm(timeout)
    best_sm = 0
    try:
        for sm in sim_measure(G1, G2):
            print("Candidate sim measure: %f" % sm)
            best_sm = max(best_sm, sm)
    except TimeoutError:
        pass
    finally:
        signal.alarm(0)

    return best_sm

def load_graphs(columns, rows):
    print("Reading column labels from %s" % columns)
    clist = load_graphs_list(columns)
    print("Reading row labels from %s" % rows)
    rlist = load_graphs_list(rows)
    return clist, rlist

def load_graphs_list(listpath):
    paths = [gpath.strip() for gpath in open(listpath, 'r')]
    return [(p.split('/')[-1], load_graph(p)) for p in paths]

def load_graph(path):
    G = nx.DiGraph(read_dot(path))
    for n in G.nodes:
        G.nodes[n]["name"] = n
        G.nodes[n]["out_degree_eq"] = len([v for (u, v) in G.edges if u == n and edge_label_operator(G.edges[(u, v)]) == "=="])
        G.nodes[n]["out_degree_neq"] = len([v for (u, v) in G.edges if u == n and edge_label_operator(G.edges[(u, v)]) == "!="])
        G.nodes[n]["out_degree_oth"] = len([v for (u, v) in G.edges if u == n and edge_label_operator(G.edges[(u, v)]) == "other"])
        G.nodes[n]["in_degree_eq"] = len([u for (u, v) in G.edges if v == n and edge_label_operator(G.edges[(u, v)]) == "=="])
        G.nodes[n]["in_degree_neq"] = len([u for (u, v) in G.edges if v == n and edge_label_operator(G.edges[(u, v)]) == "!="])
        G.nodes[n]["in_degree_oth"] = len([u for (u, v) in G.edges if v == n and edge_label_operator(G.edges[(u, v)]) == "other"])
    for e in G.edges:
        u, v = e
        G.edges[e]["src"] = G.nodes[u]
        G.edges[e]["dst"] = G.nodes[v]
    return G

def sim_measure(G1, G2):
    for distance in optimize_graph_edit_distance(G1, G2,
            node_subst_cost=node_subst_cost,
            node_del_cost=lambda e: 2,
            node_ins_cost=lambda e: 2,
            edge_subst_cost=edge_subst_cost,
            edge_del_cost=lambda e: 2,
            edge_ins_cost=lambda e: 2):
        max_distance = 8*(max(len(G1.nodes),len(G2.nodes)) + max(len(G1.edges),len(G2.edges)) + 1)
        yield 1 - (distance / max_distance)

def funcdebug(func):
    def wrapper(*args, **kwargs):
        ret = func(*args, **kwargs)
        if False: #ret > 2:
            print("calling %s(%s) -> %s" % (func.__name__, str(args) + str(kwargs), ret))
        return ret
    wrapper.__name__ = func.__name__
    return wrapper

@funcdebug
def node_subst_cost(n1, n2):
    l1 = n1["label"];
    l2 = n2["label"];
    prog = re.compile('"FunctionInvocation \(calling-line=regs\.rax = (?P<funcname>[^(]+) \(')
    r1 = prog.match(l1)
    r2 = prog.match(l2)
    if r1 is None and r2 is None:
        cost = 0
    elif r1 is None or r2 is None:
        cost = 1
    else:
        cost = funcname_matching(r1.group("funcname"), r2.group("funcname"))
    return cost + degrees_matching_cost(n1, n2)

def degrees_matching_cost(n1, n2):
    cout_eq = abs(n1["out_degree_eq"] - n2["out_degree_eq"]) + 1
    cout_neq = abs(n1["out_degree_neq"] - n2["out_degree_neq"]) + 1
    cout_oth = abs(n1["out_degree_oth"] - n2["out_degree_oth"]) + 1
    cin_eq = abs(n1["in_degree_eq"] - n2["in_degree_eq"]) + 1
    cin_neq = abs(n1["in_degree_neq"] - n2["in_degree_neq"]) + 1
    cin_oth = abs(n1["in_degree_oth"] - n2["in_degree_oth"]) + 1
    return cout_eq * cout_neq * cout_oth * cin_eq * cin_neq * cin_oth - 1

def funcname_matching(n1, n2):
    if n1 == "puts":
        n1 = "printf"
    if n2 == "puts":
        n2 = "printf"
    if n1 == n2:
        return 0
    else:
        return 1

@funcdebug
def edge_subst_cost(e1, e2):
    l1 = e1["label"];
    l2 = e2["label"];
    prog = re.compile('.*(?P<operator>!=|==)', re.DOTALL)
    r1 = prog.match(l1)
    r2 = prog.match(l2)
    if r1 is None and r2 is None:
        cost = operands_set_cost(l1, l2)
    elif r1 is None or r2 is None:
        cost = 1
    elif r1.group("operator") == r2.group("operator"):
        cost = operands_set_cost(l1, l2)
    else:
        cost = 1
    return cost + src_dst_matching_cost(e1, e2)

def edge_label_operator(e):
    l = e["label"];
    prog = re.compile('.*(?P<operator>!=|==)', re.DOTALL)
    op = prog.match(l)
    if op is None:
        return "other"
    return op.group("operator")

def src_dst_matching_cost(e1, e2):
    c1 = node_subst_cost(e1["src"], e2["src"]) + 1
    c2 = node_subst_cost(e1["dst"], e2["dst"]) + 1
    return c1 * c2 - 1

def operands_set_cost(l1, l2):
    prog = re.compile(r'(?<!\w)([renm]\w*|cs|ss|ds|es|fs|gs)(?!\w)')
    vars1 = set(m.group(0) for m in prog.finditer(l1))
    vars2 = set(m.group(0) for m in prog.finditer(l2))
    common_vars = vars1.intersection(vars2)
    if len(vars1) == 0 and len(vars2) == 0:
        return 0
    return (len(vars1) + len(vars2) - 2*len(common_vars)) / (len(vars1) + len(vars2) - len(common_vars))



if __name__ == "__main__":
    main()
