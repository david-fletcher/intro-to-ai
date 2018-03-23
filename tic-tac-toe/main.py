import random
import copy

BOARD = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
PLAYER_SYMBOL = 'H'
COMPUTER_SYMBOL = 'C'
DISASTER_AVOIDANCE = (-1, -1)
POSSIBLE_TURN = (-1, -1)
BASE_EXIT = 5

turn = PLAYER_SYMBOL


def main():
    turn_counter = initialize_game()
    global turn

    while not has_won(BOARD, turn) and not is_board_full(BOARD):
        turn_counter += 1
        print_board()
        if turn_counter % 2:
            turn = PLAYER_SYMBOL
            print("---------")
        else:
            turn = COMPUTER_SYMBOL
            print("---------")

        get_move(turn)

    print_board()
    if(has_won(BOARD, turn)):
        print("%s has won!" % turn)
    elif(is_tie()):
        print("Match is a tie!")
    else:
        print("An error occured.")
    return 0


def print_board():
    print("%s %s %s" % tuple(BOARD[0]))
    print("%s %s %s" % tuple(BOARD[1]))
    print("%s %s %s" % tuple(BOARD[2]))


def check_rows(board, player):
    for row in range(0, 3, 1):
        if board[row][0] == player and board[row][1] == player and board[row][2] == player:
            return True

    return False


def check_cols(board, player):
    for col in range(0, 3, 1):
        if board[0][col] == player and board[1][col] == player and board[2][col] == player:
            return True

    return False


def check_diags(board, player):
    if board[0][0] == player and board[1][1] == player and board[2][2] == player:
        return True
    if board[0][2] == player and board[1][1] == player and board[2][0] == player:
        return True
    return False


def has_won(board, player):
    if check_cols(board, player) or check_diags(board, player) or check_rows(board, player):
        return True

    return False


def initialize_game():
    first_move = input("Who will be playing first? (H or C): ")
    if first_move == PLAYER_SYMBOL:
        return 0
    elif first_move == COMPUTER_SYMBOL:
        return 1
    else:
        print("You didn't choose who will go first! Quitting...")


def get_move(t):
    if t == PLAYER_SYMBOL:
        get_human_move()
    else:
        get_comp_move()


def get_human_move():
    move = input("%s, place your next move (r c) (quit to exit): " % turn)

    if move == "quit" or move == "Quit" or move == "q":
        print("Quitting before I win? Seeya later!")
        quit()

    move_list = move.split()
    if(len(move_list) != 2):
        print("Not enough values were given! (r c): ")
        return get_human_move()

    try:
        row = int(move_list[0])
        col = int(move_list[1])
        if row > 2 or col > 2:
            raise ValueError
        if row < 0 or col < 0:
            raise ValueError
        if BOARD[row][col] != 0:
            raise IOError
        BOARD[row][col] = turn

    except ValueError:
        print("Wrong value detected! Try again!")
        return get_human_move()

    except IOError:
        print("You can't play there! Try again!")
        return get_human_move()

    except KeyboardInterrupt:
        print("Quitting before I win? Seeya later!")


def get_comp_move():
    valid_move = False

    # calced_move = can_win(copy.deepcopy(BOARD))
    # if calced_move != (-1, -1):
    #     BOARD[calced_move[0]][calced_move[1]]
    #     return

    while not valid_move:
        # row = random.randrange(0, 3)
        # col = random.randrange(0, 3)

        find_next_move(BOARD, COMPUTER_SYMBOL, 0, 0)
        row = POSSIBLE_TURN[0]
        col = POSSIBLE_TURN[1]
        if(DISASTER_AVOIDANCE != (-1, -1)):
            row = DISASTER_AVOIDANCE[0]
            col = DISASTER_AVOIDANCE[1]

        if BOARD[row][col] == 0:
            BOARD[row][col] = COMPUTER_SYMBOL
            valid_move = True
        else:
            place_random()
            valid_move = True

# def can_win(board):
#     for row in range(0, 3, 1):
#         for col in range(0, 3, 1):
#             if board[row][col] == 0:
#                 board[row][col] = COMPUTER_SYMBOL
#                 if has_won(tempBoard):
#                     return (row, col)
#                 else:
#                     won = can_win(board)
#                     if(won != (-1, -1)):
#
#
#     return (-1,-1)

def place_random():
    valid_move = False
    while not valid_move:
        row = random.randrange(0, 3)
        col = random.randrange(0, 3)

        if BOARD[row][col] == 0:
            BOARD[row][col] = COMPUTER_SYMBOL
            valid_move = True

def is_tie():
    comp_win = has_won(BOARD, COMPUTER_SYMBOL)
    hum_win = has_won(BOARD, PLAYER_SYMBOL)
    board_full = is_board_full(BOARD)
    if not comp_win and not hum_win and board_full:
        return True
    return False

def is_board_full(board):
    for row in range(0, 3, 1):
        for col in range(0, 3, 1):
            if(board[row][col] == 0):
                return False

    return True


def find_next_move(board, team_symbol, lastRow, lastCol):
    global POSSIBLE_TURN
    tempBoard = copy.deepcopy(board)
    if base_case(board) != BASE_EXIT:
        return base_case(board)
    else:
        #can we win on the next move?
        for row in range(0, 3, 1):
            for col in range(0, 3, 1):

                if board[row][col] == 0:
                    tempBoard[row][col] = team_symbol
                    baseResult = base_case(tempBoard)
                    if baseResult == -1:
                        return -1
                    elif baseResult == 1:
                        POSSIBLE_TURN = (row, col)
                        return 1
                    else:
                        tempBoard[row][col] = 0

        #if we can't win next move
        for row in range(0, 3, 1):
            for col in range(0, 3, 1):
                if board[row][col] == 0:
                    tempBoard[row][col] = team_symbol
                    previousResult = find_next_move(tempBoard, get_next_team_symbol(team_symbol), row, col)
                    if previousResult == -2:
                        global DISASTER_AVOIDANCE
                        if DISASTER_AVOIDANCE == (-1, -1):
                            DISASTER_AVOIDANCE = (lastRow, lastCol)
                            return -2
                    elif previousResult == -1:
                        tempBoard[row][col] = 0 # Unplace piece
                        continue
                    elif previousResult == 0:
                        tempBoard[row][col] = 0 # Unplace piece
                        POSSIBLE_TURN = (row, col)
                        continue
                    else: #previousResult == 1
                        POSSIBLE_TURN = (row, col)
                        return 1

        return 0

def get_next_team_symbol(team_symbol):
    if team_symbol == COMPUTER_SYMBOL:
        return PLAYER_SYMBOL
    else:
        return COMPUTER_SYMBOL

def base_case(board):
    if has_won(board, COMPUTER_SYMBOL):
        return 1
    elif (check_rows(board, PLAYER_SYMBOL) and check_cols(board, PLAYER_SYMBOL)) or (check_cols(board, PLAYER_SYMBOL) and check_diags(board, PLAYER_SYMBOL)) or (check_diags(board, PLAYER_SYMBOL) and check_rows(board, PLAYER_SYMBOL)):
        return -2
    elif has_won(board, PLAYER_SYMBOL):
        return -1
    elif is_board_full(board):
        return 0
    else:
        return BASE_EXIT


main()
