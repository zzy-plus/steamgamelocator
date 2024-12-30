from sglocator import get_game_location

game_folder_name = 'GarrysMod'
try:
    paths = get_game_location(game_folder_name)
    for path in paths:
        print(path)
except Exception as e:
    print(e)


