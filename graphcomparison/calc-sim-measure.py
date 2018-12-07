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
        G.nodes[n]["out_edges"] = [edge_operator_operands(G.edges[e]) for e in G.edges if e[0] == n]
        G.nodes[n]["in_edges"] = [edge_operator_operands(G.edges[e]) for e in G.edges if e[1] == n]
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
    return cost + edges_matching_cost(n1, n2)

def edges_matching_cost(n1, n2):
    if len(n1["out_edges"]) <= len(n2["out_edges"]):
        cout = greedy_edges_matching_cost(n1["out_edges"], n2["out_edges"])
    else:
        cout = greedy_edges_matching_cost(n2["out_edges"], n1["out_edges"])
    if len(n1["in_edges"]) <= len(n2["in_edges"]):
        cin = greedy_edges_matching_cost(n1["in_edges"], n2["in_edges"])
    else:
        cin = greedy_edges_matching_cost(n2["in_edges"], n1["in_edges"])
    return (cout+1) * (cin+1) - 1

def greedy_edges_matching_cost(el1, el2):
    """
    The el1 and el2 are two lists with [(operator:str, operands:set)]
    format and el1 does not have more items than el2.
    Starting from first item in el1, summing the minimum cost which
    can map that item to some item in el2 (and removing the
    corresponding minimum cost item from el2) while adding 1 for the
    cost of missing edges in the el1 (since len(el1)<=len(el2)), the
    resulting cost is returned as a greedy similarity measure among
    the two lists.
    """
    def find_and_extract_min_cost_match(op1, vars1, remaining_edges):
        min_cost = None
        min_e = None
        next_remaining_edges = []
        for cur_e in remaining_edges:
            op2, vars2 = cur_e
            if op1 == "other" and op2 == "other":
                cost = vars_cost(vars1, vars2)
            elif op1 == "other" or op2 == "other":
                cost = 1
            elif op1 == op2:
                cost = vars_cost(vars1, vars2)
            else:
                cost = 1
            if min_cost is None or cost < min_cost:
                min_cost = cost
                if min_e is not None:
                    next_remaining_edges.append(min_e)
                min_e = (op2, vars2)
            else:
                next_remaining_edges.append(cur_e)
        return min_cost, next_remaining_edges
    cost = 0
    remaining_edges = el2
    for op1, vars1 in el1:
        min_cost, remaining_edges = \
                find_and_extract_min_cost_match(op1, vars1, remaining_edges)
        cost = cost + min_cost
    cost = cost + len(remaining_edges)
    return cost

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

def edge_operator_operands(e):
    l = e["label"];
    prog = re.compile(r'(?<!\w)([renm]\w*|cs|ss|ds|es|fs|gs)(?!\w)')
    variables = set(m.group(0) for m in prog.finditer(l))
    m2 = re.match('.*(?P<operator>!=|==)', l, re.DOTALL)
    operator = "other" if m2 is None else m2.group("operator")
    return operator, variables

def src_dst_matching_cost(e1, e2):
    c1 = node_subst_cost(e1["src"], e2["src"]) + 1
    c2 = node_subst_cost(e1["dst"], e2["dst"]) + 1
    return c1 * c2 - 1

def operands_set_cost(l1, l2):
    prog = re.compile(r'(?<!\w)([renm]\w*|cs|ss|ds|es|fs|gs)(?!\w)')
    vars1 = set(m.group(0) for m in prog.finditer(l1))
    vars2 = set(m.group(0) for m in prog.finditer(l2))
    return vars_cost(vars1, vars2)

def vars_cost(vars1, vars2):
    common_vars = vars1.intersection(vars2)
    if len(vars1) == 0 and len(vars2) == 0:
        return 0
    return (len(vars1) + len(vars2) - 2*len(common_vars)) / (len(vars1) + len(vars2) - len(common_vars))


if __name__ == "__main__":
    main()
