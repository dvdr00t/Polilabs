exports.writeJson = (code, payload) => {

	let isJsonObject = (strData) => {
		try {
			JSON.parse(strData);
		} catch (e) {
			return false;
		}
		return true;
	}

	// if (isJsonObject(payload)) {
	// 	return {
	// 		code: code,
	// 		message: payload
	// 	}
	// }

	return {
		code: code,
		message: payload
	}
	
}