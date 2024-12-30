const {getGameLocation} = require('./sglocator')

const gameFolderName = "GarrysMod"
try{
    const paths = getGameLocation(gameFolderName)
    for (const path of paths) {
        console.log(path)
    }
}catch (e){
    console.log(e)
}

