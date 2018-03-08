import random

BOARD = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
PLAYER_SYMBOL = 'H'
COMPUTER_SYMBOL = 'C'

turn = PLAYER_SYMBOL


def main():
    turn_counter = initialize_game()
    global turn

    while not has_won():
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
    print("%s has won!" % turn)
    return 0


def print_board():
    print("%s %s %s" % tuple(BOARD[0]))
    print("%s %s %s" % tuple(BOARD[1]))
    print("%s %s %s" % tuple(BOARD[2]))


def check_rows():
    for row in range(0, 2, 1):
        if BOARD[row][0] == turn and BOARD[row][1] == turn and BOARD[row][2] == turn:
            return True

    return False


def check_cols():
    for col in range(0, 2, 1):
        if BOARD[0][col] == turn and BOARD[1][col] == turn and BOARD[2][col] == turn:
            return True

    return False


def check_diags():
    if BOARD[0][0] == turn and BOARD[1][1] == turn and BOARD[2][2] == turn:
        return True
    if BOARD[0][2] == turn and BOARD[1][1] == turn and BOARD[2][0] == turn:
        return True
    return False


def has_won():
    if check_cols() or check_diags() or check_rows():
        return True

    return False


def initialize_game():
    first_move = raw_input("Who will be playing first? (H or C): ")
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
    move = raw_input("%s, place your next move (r c) (quit to exit): " % turn)

    if move == "quit" or move == "Quit" or move == "q":
        print("Quitting before I win? Seeya later!")
        quit()

    move_list = move.split()

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

    except IOError:
        print("You can't play there! Try again!")

    except KeyboardInterrupt:
        print("Quitting before I win? Seeya later!")


def get_comp_move():
    valid_move = False

    while not valid_move:
        row = random.randrange(0, 3)
        col = random.randrange(0, 3)

        if BOARD[row][col] == 0:
            BOARD[row][col] = COMPUTER_SYMBOL
            valid_move = True


main()
