import numpy as np
from collections import deque

# Genertes a crossword puzzle
# words: list of words
# positions: list of the positions and orientations of each word
# height: grid height
# width: grid width
def generate_puzzle(words, height, width):
    sorted_words = sort_words(words)
    grid = find_optimal(sorted_words, 0, np.empty((height, width), str), 0)
    return grid

# Generates a list of puzzles using slightly different word orders
# words: list of words
# positions: list of the positions and orientations of each word
# height: grid height
# width: grid width
def generate_puzzles(words, height, width, amount):
    sorted_words = sort_words(words)
    rng = np.random.default_rng()
    grids = []
    for i in range(0, amount):
        current_words = sorted_words[:]
        for j in range(0, len(current_words) - 1, 2):
            # 30% chance to swap positions of 2 words
            if rng.uniform(0, 1) > 0.7:
                current_words[j] = sorted_words[j + 1]
                current_words[j + 1] = sorted_words[j]
        grids.append(find_optimal(current_words, 0, np.empty((height, width), str), 0))

    return grids

# Returns a list of words sorted in descending order by the number of common characters they have with the other words
def sort_words(words):
    tuple_list = []
    for i, word in enumerate(words):
        sum = 0
        for j, other_word in enumerate(words):
            if i != j:
                for l in word:
                    for l2 in other_word:
                        if l == l2:
                            sum += 1
        index = 0
        for word_tuple in tuple_list:
            if word_tuple[0] >= sum:
                index += 1
        tuple_list.insert(index ,(sum, word))

    return [tup[1] for tup in tuple_list]


# Tries to find the best puzzle that can be made with the given list of words and grid sizes
# Returns the positions of the starts of each of the given words in the same order as the word order
# Positions are a tuple of the form (row, columm, is_vertical)
# words: list of words that make up the puzzle
# positions: list of tuples of the form (row, columm, is_vertical)
# height: grid height
# width: grid width
def find_optimal(words, list_index, board, score):
    height = board.shape[0]
    width = board.shape[1]
    # If we placed all our words calculate the board score
    if(list_index >= len(words)):
        return (score, board)
    # Step to all the possible positions a word can be in
    possible_next_boards = []
    for orientation in (True, False):
        for i in range(0, height):
            for j in range(0, width):
                new_board = np.copy(board)
                # For every valid placement add it to a list with its score
                word_score = try_place_word(words[list_index], (i, j, orientation), new_board, words)
                if(word_score > 0 or (list_index == 0 and word_score > -1)):
                    possible_next_boards.append(find_optimal(words, list_index + 1, new_board, score + word_score))

    # No possible positions for the next word, reutrn a score of 0 and an empty position list. 
    if(len(possible_next_boards) == 0):
        return(0, [])
    
    # Return word position list with maximum score
    best_positions = possible_next_boards[0]
    for score_positions in possible_next_boards:
        if score_positions[0] > best_positions[0]:
            best_positions = score_positions

    return best_positions
    
    

# Tries to place a word in a given position
# Returns the amount of crossovers if successful and 0 otherwise
# word: word to be placed
# position: tuple of coordinates of the words first letter and its orientation
# grid: the grid we place the word into
# words: list of words in the puzzle(for crossover checks)
def try_place_word(word, position, grid, words):
    score = 0
    # Check that we are within bounds
    if(position[2] and position[0] + len(word) > grid.shape[0]):
        return -1
    if(not(position[2]) and position[1] + len(word) > grid.shape[1]):
        return -1
    
    row = position[0]
    column = position[1]

    # Check for a clear space or border at the start and end
    if(position[2]):
        if(row > 0 and grid[row - 1][column] != np.str_('') or
           (row + len(word) < grid.shape[0] - 1 and grid[row + len(word)][column] != np.str_(''))):
            return -1
    else:
        if(column > 0 and grid[row][column - 1] != np.str_('') or
            (column + len(word) < grid.shape[1] - 1 and grid[row][column + len(word)] != np.str_(''))):
            return -1

    for char in word:
        # Check that all characters in our path are the same as the ones we try to place
        if(char != grid[row][column] and grid[row][column] != np.str_('')):
            return -1

        # Check for crossovers
        crossover_stat = check_crossover(row, column, not(position[2]), grid, words, char)
        if(crossover_stat == 2): 
            score += 1
        elif(crossover_stat == 0):
            return -1

        grid[row][column] = char
        # Step ahead according to orientation true: vertical, false: horizonal
        if(position[2]):
            row += 1
        else:
            column += 1 
        
    return score

# Checks if a position has a valid crossover
# row: our current row
# column: our current column
# vertical: are we looking for a vertical word
# grid: 2d array representing our grid
# words: list of the words in the puzzle we check against
# start_char: The character that we are trying to insert from the new word
def check_crossover(row, column, vertical, grid, words, start_char):
    spot = column
    limit = grid.shape[1]
    if(vertical):
        limit = grid.shape[0]
        spot = row

    curr_row = row
    curr_column = column

    char_deque = deque(start_char)

    i = -1
    if(vertical):
        curr_row -= 1
    else:
        curr_column -= 1

    while spot + i >= 0 and grid[curr_row][curr_column] != np.str_(''):
        char_deque.appendleft(grid[curr_row][curr_column])
        if(vertical):
            curr_row -= 1
        else:
            curr_column -= 1
        i -= 1

    curr_row = row
    curr_column = column

    i = 1
    if(vertical):
        curr_row += 1
    else:
        curr_column += 1
    while spot + i < limit and grid[curr_row][curr_column] != np.str_(''):
        char_deque.append(grid[curr_row][curr_column])
        if(vertical):
            curr_row += 1
        else:
            curr_column += 1
        i += 1

    crossed_word = "".join(char_deque)

    if(len(crossed_word) <= 1):
        return 1
    
    if(crossed_word in words):
        return 2
    
    return 0

# Build a grid
# words: Words to build with
# positions: Positions of those words(same order as the words)
# height: height of the grid
# width: width of the grid
def build_grid(words, positions, height, width):
    edge = min(len(words), len(positions))
    grid = np.empty((height, width), str)
    for i in range(0, edge):
        place_word(words[i], positions[i], grid)

    return grid

# Place a word using the given position/orientation tuple into the given grid
# word: word to be placed
# position: tuple of coordinates of the words first letter and its orientation
# grid: 2 dimensional array representing our grid
def place_word(word, position, grid):
    row = position[0]
    column = position[1]
    for i in range(0, len(word)):
        grid[row][column] = word[i]
        if(position[2]):
            row += 1
        else:
            column += 1

# Scores the grid by counting the number of crossing points
# grid: 2 dimensional array representing our grid
def score_grid(grid):
    score = 0
    for i in range(0, grid.shape[0]):
        for j in range(0, grid.shape[1]):
            cross_v = False
            cross_h = False
            # Check for letters bellow or above
            if((i > 0 and grid[i - 1][j] != np.str_('')) or
               (i < grid.shape[0] - 1 and grid[i + 1][j] != np.str_(''))):
                cross_v = True
            # Check for letters to the left or right
            if((j > 0 and grid[i][j - 1] != np.str_('')) or
               (j < grid.shape[1] - 1 and grid[i][j + 1] != np.str_(''))):
                cross_h = True
            # If we both a horizontal and a vertical neighbour we are at a crossing point
            if(cross_h and cross_v):
                score += 1

    return score

def show_board(board):
    for i in range(0, board.shape[0]):
        line = "|"
        for j in range(0, board.shape[1]):
            character = board[i][j]
            if(character == np.str_('')):
                character = ' '
            line += character + "|"
        print(line)
    
word_list = ["coldplay", "chemical",  "reaction", "yellow", "elton", "hole", "space", "jupiter"]

boards = generate_puzzles(word_list, 10, 12, 2)

for board in boards:
    show_board(board[1])
    print("score: " + str(board[0]))