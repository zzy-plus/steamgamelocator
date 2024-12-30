const {getGameLocation} = require('./sglocator')

const test = async()=>{
	const gameFolderName = "GarrysMod"
	try{
		const paths = await getGameLocation(gameFolderName)
		for (const path of paths) {
			console.log(path)
		}
	}catch (e){
		console.log(e)
	}
}

test()
