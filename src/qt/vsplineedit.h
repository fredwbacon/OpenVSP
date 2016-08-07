#ifndef VSPLINEEDIT_H
#define VSPLINEEDIT_H

#include "vspglobal.h"

#include <QtCore>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#else
#include <QtGui>
#endif

namespace Ui {
  class FileInput;
  class DirectoryInput;
}

class VSP_EXPORT vspDoubleLineEdit : public QLineEdit
{
  Q_OBJECT
  public:
    vspDoubleLineEdit ( QWidget* p=0 );
    ~vspDoubleLineEdit (  );

    void focusOutEvent ( QFocusEvent * );

    double value (  ) const;

  public slots:
    void setValue ( double );

  signals:
    void valueChanged ( double );

  protected:
    double d_value;
    QDoubleValidator *validator;
};


class VSP_EXPORT vspIntegerLineEdit : public QLineEdit
{
  Q_OBJECT
  public:
    vspIntegerLineEdit ( QWidget* p=0 );
    ~vspIntegerLineEdit (  );

    void focusOutEvent ( QFocusEvent * );

    int value (  ) const;

  public slots:
    void setValue ( int );

  signals:
    void valueChanged ( int );

  protected:
    int d_value;
    QIntValidator *validator;
};


class VSP_EXPORT vspFieldEdit : public QLineEdit
{
  Q_OBJECT
  public:
    vspFieldEdit ( QWidget* p=0, const QString& mask=">NNNNN;" );
    ~vspFieldEdit (  );

  public slots:
    void watchText ( const QString& );

  signals:
    void valueReady ( const QString& );

  protected:
    int  d_fieldWidth;
};


class VSP_EXPORT vspFileInput : public QWidget
{
  Q_OBJECT
  Q_PROPERTY( QString filename READ filename WRITE setFilename );
  Q_PROPERTY( QString baseDirectory READ baseDirectory WRITE setBaseDirectory );
  Q_PROPERTY( bool useRelative READ useRelative WRITE setUseRelative );

  public:
    vspFileInput ( QWidget* p=0, const QStringList& f=QStringList() );
    ~vspFileInput (  ); 

    QString filename (  ) const
      { return d_filename; }

    void setFilename ( const QString& s );

    QString baseDirectory () const
      { return d_baseDirectory; }

    void setBaseDirectory ( const QString& bd )
      { d_baseDirectory = bd; }

    bool useRelative ( ) const
      { return d_useRelative; }

    void setUseRelative ( const bool ur )
      { d_useRelative = ur; }

  public slots:
    void setFilters ( const QStringList& f );
    void setFileName ( const QString& );
    void onBrowse (  );
    void updateText ( const QString& );

  signals:
    void textChanged ( const QString& );

  protected:
    QStringList  d_filters;
    QString      d_filename;
    QString      d_baseDirectory;
    bool         d_useRelative;

  private:
    Ui::FileInput*  ui;
};

class VSP_EXPORT vspDirectoryInput : public QWidget
{
  Q_OBJECT
  Q_PROPERTY( QString dirname READ dirname WRITE setDirname );
  public:
    vspDirectoryInput ( QWidget* p=0 );
    ~vspDirectoryInput (  );

    QString dirname (  ) const
      { return d_dirname; }

    void setDirname ( const QString& s )
      { d_dirname = s; }

  public slots:
    void setDirectoryName ( const QString& );
    void onBrowse (  );
    void updateText ( const QString& );

  signals:
    void textChanged ( const QString& );

  protected:
    QString      d_dirname;

  private:
   Ui::DirectoryInput* ui;
};


#endif // VSPLINEEDIT_H
