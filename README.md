# RepLinkApk Manager
 Make your own local or online repositories of Android apps on Windows. Easy [F-Droid][f-droid] like repo structure and using it's [Original App][fdroid-app] as store. Also an easy way to get info from an APK file.
 
 [![RepLinkApk-Manager-Video](//img.youtube.com/vi/qHImm0yUIHY/0.jpg)](//www.youtube.com/watch?v=qHImm0yUIHY&t=0s "Manager Basic Tutorial")
 
 [f-droid]: https://f-droid.org/
 [fdroid-app]: https://f-droid.org/packages/org.fdroid.fdroid/

## Requirements
 In case of you want to compile...
 This is a portable win32 app, so just an .exe file, but next to it:
* A [sqlite database][sqlite-db] where is stored the repositories config among others.
* A folder with [`keystores`][keystore] for signing the repositories
* A folder `bins` with this 3 [files][bin-files]
* An normal install of [JDK 1.8.0][jdk-link]. Note: This one is tested, other versions could have issues.

[sqlite-db]: https://github.com/tomriddle537/RepLinkApk-Manager/blob/master/assets/config.sqlitedb
[keystore]: https://github.com/tomriddle537/RepLinkApk-Manager/blob/master/assets/keystores/RepLinkApk.ks
[bin-files]: https://drive.google.com/drive/folders/1x-84SevvpjKgTZZi0EGsgDv93TSEAFTA?usp=sharing
 In case you use the release you only have to install [JDK 1.8.0][jdk-link]...

For Windows, a prebuilt archive with all the dependencies is available:

 - [`RepLinkApk-v1.0.1.zip`][direct-win32]  

[direct-win32]: https://github.com/tomriddle537/RepLinkApk-Manager/releases/download/v1.0.1/RepLinkApk-v1.0.1.zip


[jdk-link]: https://www.oracle.com/java/technologies/javase/javase-jdk8-downloads.html

 I'm assuming you will use a web server like Apache or Nginx later, to make your repository accessible with the F-Droid [Original App][fdroid-app].
 
 
### Making your own keystores
 When setting up the repository, one of the first steps should be to generate a signing key for the repository index. This will also create a keystore, which is the file that holds the signing keys. You can use JDKs keytool.exe

 keytool -genkey -v -keystore C:/mykeystore.ks -alias repokey \
  -keyalg RSA -keysize 2048 -validity 10000

 In the above, replace ’mykeystore.ks’ with the name of the keystore file to be created, and ’repokey’ with a name to identify the repo index key by. You’ll be asked for a password for the keystore, AND a password for the key. They shouldn’t be the same. In between, you’ll be asked for some identifying details which will go in the certificate.

 The keystore config goes into <kbd>Config</kbd>/<kbd>Options</kbd>, as the ’repokey’ alias, keystorepass and keypass respectively. The path to the keystore file is set in ’keystores’ folder.
 
 # Basic Usage 
 
 1. Install JDK 1.8.0
 1. Config your own keystore or use the one in [`keystores`][keystore] folder. For the provided keystore set Alias to "Manager", keystorepass to "RepLinkApk" and keypass to "Manager". <kbd>Config</kbd>/<kbd>Options</kbd>
 1. Create a repository in any empty folder and add some categories for your android apps.  <kbd>Repository</kbd>/<kbd>Repositories Manager</kbd>
 1. Add some apps to the `For Process Apps` area. <kbd>Repository</kbd>/<kbd>Add Sended APK files</kbd>
 1. Double click each one and select a category.
 1. Hit the <kbd>Process</kbd> button and wait.
 1. Done
 
 ## Features
 
 1. `Create/Edit/Open/Backup/Close/Delete` repositories and categories <kbd>Repository</kbd>/<kbd>Repositories Manager</kbd>
 1. `APK files viewer` a explorer like tool for browsing folders with apks <kbd>View</kbd>/<kbd>APK files viewer</kbd>
 1. `Apps in repository` a list view for Browse/Edit/Delete apps and packages in an Opened repository. <kbd>View</kbd>/<kbd>Apps in repository</kbd>
 1. `Apps per Category count`. <kbd>View</kbd>/<kbd>Apps per Category count</kbd>
 1. `Apk Info View` a tool that works even outside the app. Make your apks `open with` the RepLinkApk.exe file. <kbd>Tools</kbd>/<kbd>Apk Info View</kbd>
 1. `Apk report` a tool to find Unused, Doble and Missing apks from your repository. <kbd>Tools</kbd>/<kbd>Apk report</kbd>
 1. `Sign Repo` for signing the repo any time you want with your key. <kbd>Tools</kbd>/<kbd>Sign Repo</kbd>
 1. `Generate JSON` for signing the repo any time you want with your key and also generate a required file for the F-Droid [Original App][fdroid-app]. <kbd>Tools</kbd>/<kbd>Generate JSON</kbd>
 1. `Merge repo/Substract repo/Delete All Packages/Generate Web Link/Set Summary like Category` a set of advanced tools for specific cases. <kbd>Tools</kbd>/<kbd>Advanced</kbd>/<kbd>Etc...</kbd>
 1. And other minor improvments.
 
 
 # Finishing
* Developer: [@tomriddle537][developer]
* License: GPLv3
* Credits to [F-Droid][f-droid] for the repository structure idea and the their [Original App][fdroid-app].
Share this with your friends and feel free to buy me a cup of coffee. ;) 
 * Bitcoin:
 bc1qpd4kw5ca8vva62rfldp6vakm84reu9shtgluzw
 * ETH:
 0xdEb6B5f1d3c3f19a936953CB4a17F2F8268AB24D

[developer]: https://github.com/tomriddle537/

 ## Note
The program is provided AS IS with NO WARRANTY OF ANY KIND.
 
 
