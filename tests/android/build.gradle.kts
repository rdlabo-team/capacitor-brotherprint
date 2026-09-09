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
            kotlin.include("jp/rdlabo/capacitor/plugin/brotherprint/BluetoothPrinterFilter.kt")
        }
        test {
            kotlin.srcDir("../../android/src/test/java")
            kotlin.include("jp/rdlabo/capacitor/plugin/brotherprint/BluetoothPrinterClassTest.kt")
        }
    }
}

dependencies {
    testImplementation("junit:junit:4.13.2")
}
