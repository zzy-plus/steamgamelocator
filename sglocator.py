import winreg
import os
import re

def get_steam_location():
    try:
        with winreg.OpenKey(winreg.HKEY_CURRENT_USER, 'Software\\Valve\\Steam') as reg_key:
            value, regtype = winreg.QueryValueEx(reg_key, 'SteamPath')
            return value
    except FileNotFoundError as e:
        raise RuntimeError('Can Not Find Steam Installation Location') from e
    except Exception as e:
        raise RuntimeError('Can Not Find Steam Installation Location') from e

def get_library_folders():
    steam_location = get_steam_location()
    librayfolders_vdf = f'{steam_location}\\config\\libraryfolders.vdf'
    if not os.path.exists(librayfolders_vdf):
        raise RuntimeError('libraryfolders.vdf Not Found')
    with open(librayfolders_vdf, 'r') as f:
        content = f.read()
        pattern = r'"path".+"(.+)"'
        result = re.findall(pattern, content)
        library_folders = []
        for folder in result:
            common_path = folder + '\\steamapps\\common'
            if os.path.isdir(common_path):
                library_folders.append(common_path)
        return library_folders

def get_game_location(game_folder_name):
    """
    Get installation location of the game
    :param game_folder_name: Must be the right folder name of the game, which you can get by right-click on the game in steam.
    :return: locations: Paths where the game installed.
    Cautions: It may contain more than one location because of the installation or uninstallation history.
    Therefore, you need to verify for yourself which location is right.
    """
    locations = []
    folders = get_library_folders()
    for folder in folders:
        game_path = f'{folder}\\{game_folder_name}'
        if os.path.isdir(game_path):
            locations.append(game_path)
    return locations










if __name__ == '__main__':
    print(get_game_location('killingfloor2'))






