plugins {
    kotlin("jvm") version "2.2.20"
}

repositories {
    mavenCentral()
}

kotlin {
    jvmToolchain(21)
    sourceSets {
        main {
            kotlin.srcDir("../../android/src/main/java")
            kotlin.include("jp/rdlabo/capacitor/plugin/brotherprint/BluetoothPrinterFilter.kt", "jp/rdlabo/capacitor/plugin/brotherprint/PrinterValidation.kt", "jp/rdlabo/capacitor/plugin/brotherprint/PrinterModels.kt", "jp/rdlabo/capacitor/plugin/brotherprint/PrinterLifecycle.kt")
        }
        test {
            kotlin.srcDir("../../android/src/test/java")
            kotlin.include("jp/rdlabo/capacitor/plugin/brotherprint/BluetoothPrinterClassTest.kt", "jp/rdlabo/capacitor/plugin/brotherprint/PrinterValidationTest.kt", "jp/rdlabo/capacitor/plugin/brotherprint/PrinterLifecycleTest.kt")
        }
    }
}

dependencies {
    testImplementation("junit:junit:4.13.2")
}
