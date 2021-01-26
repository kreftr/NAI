from graphviz import Graph


def read_graph_from_file(file_name):
    if not file_name.endswith('.graph'):
        print(f"{file_name} is not a graph file!")
    else:
        try:
            file = open(file_name)
            adjacency_matrix = []
            edges = []

            for line in file:
                line = line.strip("\n")
                adjacency_matrix.append(line.split(","))

            print(adjacency_matrix)

        except Exception as e:
            print(e)
        finally:
            file.close()
            graph = Graph()

            for index in range(len(adjacency_matrix)):
                graph.node(str(int(index)), str(int(index)))

            for i, vertices in enumerate(adjacency_matrix):

                for j, vertex in enumerate(vertices):
                    if int(vertex) != 0 and [str(j), str(i)] not in edges:
                        graph.edge(str(i), str(j), vertex)
                        edges.append([str(i), str(j)])
            return graph


read_graph_from_file('test.graph').render('round-table.gv', view=True)
read_graph_from_file('test2.graph').render('round-table2.gv', view=True)
