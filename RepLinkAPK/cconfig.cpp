#include "cconfig.h"
#include "ui_cconfig.h"

CConfig::CConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CConfig)
{
    ui->setupUi(this);

    settings= new QSettings(QSettings::IniFormat, QSettings::UserScope,
                            "RepLink APK Manager", "RepLinkAPK");
    ui->eJava->setText(settings->value("AppDirs/java").toString());

    ui->eKeySore->setText(settings->value("KeyStoreConf/keystore").toString());
    ui->eKeyStorePass->setText(settings->value("KeyStoreConf/storepass").toString());
    ui->eKey->setText(settings->value("KeyStoreConf/key").toString());
    ui->eKeyPass->setText(settings->value("KeyStoreConf/keypass").toString());
}

CConfig::~CConfig()
{
    delete ui;
}

void CConfig::on_buttonBox_accepted()
{
    settings->setValue("AppDirs/java",ui->eJava->text());

    settings->setValue("KeyStoreConf/keystore",ui->eKeySore->text());
    settings->setValue("KeyStoreConf/storepass", ui->eKeyStorePass->text());
    settings->setValue("KeyStoreConf/key",ui->eKey->text());
    settings->setValue("KeyStoreConf/keypass",ui->eKeyPass->text());

    delete settings;
    emit finished(0);
}

void CConfig::on_tbJava_clicked()
{
    QStringList env = QStringList() <<"C:\\Program Files\\Java\\jdk1.8.0\\bin"<<"C:\\Archivos de programa\\Java\\jdk1.8.0\\bin"<<"C:\\Program Files (x86)\\Java\\jdk1.8.0\\bin";



    int pos=-1;
    for (int i = 0; i < env.length(); ++i) {
        QFileInfo java= QFileInfo(env.at(i)+"\\java.exe");
        if(java.exists()){
            pos=i;
            break;
        }
    }

    QFileDialog abrir;
    QString javaPath=(pos!=-1)? QString(env.at(pos)).replace("\\","/"):ui->eJava->text();
    QString direc= abrir.getExistingDirectory(this, tr("Abrir Ruta"), javaPath);

    QDir d(direc);

    if(d.exists()){
        QFileInfo w(d.absolutePath()+"/java.exe");
        QFileInfo x(d.absolutePath()+"/jar.exe");
        QFileInfo y(d.absolutePath()+"/jarsigner.exe");
        QFileInfo z(d.absolutePath()+"/keytool.exe");

        if(w.exists()&&x.exists()&&y.exists()&&z.exists()){
            ui->eJava->setText(QString(d.absolutePath()).replace("\\","/"));
        }else{
            QMessageBox::warning(this,"Error", "This directory it's not correct. \nThere are some missing tools.");
        }

    }

}

void CConfig::on_tbKeyStore_clicked()
{
    QDir d(settings->value("AppDirs/path").toString()+"/keystores");
    if(!d.exists()){
        QMessageBox::warning(this,"Error", "There is no keystores directory. It will be created!");
        d.mkdir(d.absolutePath());
    }

    QFileDialog abrir;
    QString manejador= abrir.getOpenFileName(this, tr("Abrir Key Store"), d.absolutePath(), tr("Key Store(*.ks)"));
    QString url=manejador.toUtf8();
    url=shortPathName(url);
    if(!url.trimmed().isEmpty()){
        QString shortDir=shortPathName(d.absolutePath());
        if(d.exists() && (shortDir!= QFileInfo(url).absoluteDir().absolutePath())){
            QMessageBox::StandardButton confirm;
            confirm = QMessageBox::question(this, "Copy to Key Store", "These Key Store file isn't in the Key Stores Directory. Do you want to copy it?", QMessageBox::Yes|QMessageBox::No);
            if(confirm == QMessageBox::Yes){
                QFile f(url);
                QFileInfo fi(url);
                QString aux=d.absolutePath()+"/"+fi.completeBaseName()+"."+fi.completeSuffix();
                if(f.copy(aux)){
                    url=aux;
                    ui->eKeySore->setText(fi.completeBaseName()+"."+fi.completeSuffix());
                }else{
                    QMessageBox::warning(this, "Error", "Couldn't copy the file!", QMessageBox::Ok);
                }
            }
        }else if(d.exists()){
            QFileInfo fi(url);
            ui->eKeySore->setText(fi.completeBaseName()+"."+fi.completeSuffix());
        }
    }


}

void CConfig::on_pushButton_pressed()
{
    ui->eKeyPass->setEchoMode(QLineEdit::Normal);
    ui->eKeyStorePass->setEchoMode(QLineEdit::Normal);
}

void CConfig::on_pushButton_released()
{
    ui->eKeyPass->setEchoMode(QLineEdit::Password);
    ui->eKeyStorePass->setEchoMode(QLineEdit::Password);
}

QString CConfig::shortPathName(const QString & file) // returns 8.3 file format from long path
{
    wchar_t* input=new wchar_t[file.size()+1];
    file.toWCharArray(input);
    input[file.size()]=L'\0'; // terminate string
    long length = GetShortPathName(input, NULL, 0);
    wchar_t* output=new wchar_t[length];
    GetShortPathName(input,output,length);
    QString ret=QString::fromWCharArray(output,length-1); // discard
    delete [] input;
    delete [] output;
    return ret;
}


