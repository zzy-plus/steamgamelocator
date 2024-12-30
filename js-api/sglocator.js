const winreg = require('winreg')
const fs = require('fs')

function getSteamLocation(){
    return new Promise((resolve, reject)=>{
        const regKey = new winreg({
            hive: winreg.HKCU,
            key: '\\Software\\Valve\\Steam'
        })
        regKey.get('SteamPath', (err, item)=>{
            if(err) throw new Error('Can Not Find Steam Installation Location')
            if(item){
                resolve(item.value)
            }else {
                throw new Error('Can Not Find Steam Installation Location')
            }
        })
    })
}

async function getLibraryFolders(){
    const steamLocation = await getSteamLocation()
    const libraryFoldersVdf = `${steamLocation}\\config\\libraryfolders.vdf`
    if(!fs.existsSync(libraryFoldersVdf)) throw new Error('libraryfolders.vdf Not Found')
    const content = fs.readFileSync(libraryFoldersVdf,{encoding: 'utf8'})
    const regex = /"path".+"(.+)"/g
    const result = content.matchAll(regex)
    const libraryFolders = []
    for (const item of result) {
        const commonPath = `${item[1]}\\steamapps\\common`
        if(!fs.existsSync(commonPath)) continue
        const stat = fs.statSync(commonPath)
        if(stat.isDirectory()){
            libraryFolders.push(commonPath)
        }
    }
    return libraryFolders
}

/**
 * Get installation location of the game.
 * @param gameFolderName Must be the right folder name of the game, which you can get by right-click on the game in steam.
 * @returns {Promise<*[]>} Paths where the game installed.
 *   Cautions: It may contain more than one location because of the installation or uninstallation history.
 *   Therefore, you need to verify for yourself which location is right.
 */
async function getGameLocation(gameFolderName){
    const folders = await getLibraryFolders()
    const locations = []
    for (const folder of folders) {
        const gamePath = `${folder}\\${gameFolderName}`
        if(!fs.existsSync(gamePath)) continue
        const stat = fs.statSync(gamePath)
        if(stat.isDirectory()){
            locations.push(gamePath)
        }
    }
    return locations
}

async function test(){

}
test()

module.exports = {
    getGameLocation
}


