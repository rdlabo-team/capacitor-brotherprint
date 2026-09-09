package jp.rdlabo.capacitor.plugin.brotherprint

internal fun validatePrinterOptions(values: Map<String, Any?>) {
    fun string(key: String, default: String? = null): String? {
        val value = values[key] ?: return default
        require(value is String) { "$key must be a string" }
        return value
    }
    fun number(key: String, default: Double? = null): Double? {
        val value = values[key] ?: return default
        require(value is Number && value.toDouble().isFinite()) { "$key must be a finite number" }
        return value.toDouble()
    }
    fun integer(key: String, default: Int): Int? {
        val value = number(key, default.toDouble())!!
        require(value % 1 == 0.0 && value <= Int.MAX_VALUE && value >= Int.MIN_VALUE) { "$key must be an integer" }
        return value.toInt()
    }

    val model = string("modelName", "QL_820NWB")!!
    require(model in printerModels) { "Unsupported model" }
    require((integer("numberOfCopies", 1) ?: 0) > 0) { "numberOfCopies must be positive" }
    require((integer("halftoneThreshold", 128) ?: -1) in 0..255) { "halftoneThreshold must be 0..255" }
    if (string("scaleMode") == "ScaleValue") require((number("scaleValue") ?: 0.0) > 0) { "scaleValue must be positive" }
    if (model.startsWith("TD")) {
        require(string("paperType") in listOf("rollPaper", "dieCutPaper", "markRollPaper")) { "Invalid paperType" }
        require(string("paperUnit", "mm") in listOf("mm", "inch")) { "Invalid paperUnit" }
        require((number("tapeWidth") ?: 0.0) > 0) { "tapeWidth must be positive" }
        if (string("paperType") != "rollPaper") require((number("tapeLength") ?: 0.0) > 0) { "tapeLength must be positive" }
        for (key in listOf("marginTop", "marginRight", "marginBottom", "marginLeft", "gapLength", "paperMarkPosition", "paperMarkLength")) {
            val value = number(key, 0.0)!!
            require(value.isFinite() && value >= 0) { "$key must be nonnegative and finite" }
        }
    }
}

internal fun requireSingleBrotherUsb(vendorIds: List<Int>) {
    require(vendorIds.count { it == 0x04f9 } == 1) { "USB requires exactly one Brother device" }
}

/** SDK USB channelInfo is empty. Expose an opaque identity from Android's permitted device instead. */
internal fun usbChannelToken(deviceName: String, vendorId: Int, productId: Int, serialNumber: String?): String {
    require(deviceName.isNotBlank()) { "USB device name is required" }
    return "usb:" + listOf(deviceName, vendorId.toString(), productId.toString(), serialNumber ?: "").joinToString("") { "${it.length}:$it" }
}

/** Keep validation/exception event and rejection ordering consistent for the native bridge. */
internal fun reportPrinterFailure(method: String, message: String, category: String, emit: (Map<String, Any>) -> Unit, reject: () -> Unit) {
    if (method == "printImage") emit(mapOf("code" to 0, "message" to message, "category" to category))
    reject()
}

internal fun bridgeChannelInfo(port: String, sdkChannelInfo: String, usbToken: String?): String {
    if (port != "usb") return sdkChannelInfo
    require(!usbToken.isNullOrBlank()) { "USB device identity is required" }
    return usbToken
}
