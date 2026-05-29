def foo(*args):
    print(bar(*args))

def bar(i, j, k):
    return i + j + k


foo(1, 2, 3)