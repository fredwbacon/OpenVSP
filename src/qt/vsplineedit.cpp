#include <iostream>
#include "ui_fileinput.h"
#include "ui_directoryinput.h"
#include "vspapplication.h"
#include "vsplineedit.h"

vspDoubleLineEdit::vspDoubleLineEdit ( QWidget *p )
    : QLineEdit ( p )
{
  validator = new QDoubleValidator ( this );
  setValidator ( validator );
}

vspDoubleLineEdit::~vspDoubleLineEdit (  )
{
  // Qt does it all for us!
}

void
vspDoubleLineEdit::setValue ( double v )
{
  QString s;
  d_value = v;
  s.sprintf ( "%g", v );
  setText ( s );
}

double
vspDoubleLineEdit::value (  ) const
{
  return d_value;
}

void
vspDoubleLineEdit::focusOutEvent ( QFocusEvent * e )
{
  if ( e->lostFocus (  ) )
    {
      if ( e->reason (  ) != Qt::ActiveWindowFocusReason
           && e->reason (  ) != Qt::PopupFocusReason )
        {
          if ( d_value != text (  ).toDouble (  ) )
            {
              d_value = text (  ).toDouble (  );
              emit valueChanged ( d_value );
            }
        }
    }

  QLineEdit::focusOutEvent ( e );
}


vspIntegerLineEdit::vspIntegerLineEdit ( QWidget *p )
    : QLineEdit ( p )
{
  validator = new QIntValidator ( this );
  setValidator ( validator );
}

vspIntegerLineEdit::~vspIntegerLineEdit (  )
{
  // Qt does it all for us!
}


void
vspIntegerLineEdit::setValue ( int v )
{
  QString s;
  d_value = v;
  s.sprintf ( "%d", v );
  setText ( s );
}


int
vspIntegerLineEdit::value (  ) const
{
   return d_value;
}

void
vspIntegerLineEdit::focusOutEvent ( QFocusEvent * e )
{
  if ( e->lostFocus (  ) )
    {
      if ( e->reason (  ) != Qt::ActiveWindowFocusReason
           && e->reason (  ) != Qt::PopupFocusReason )
        {
          if ( d_value != text (  ).toInt (  ) )
            {
              d_value = text (  ).toInt (  );
              emit valueChanged ( d_value );
            }
        }
    }

  QLineEdit::focusOutEvent ( e );
}


vspFieldEdit::vspFieldEdit ( QWidget * p, const QString& mask )
    : QLineEdit ( p )
{
  setInputMask( mask );
  d_fieldWidth = mask.length();

  connect ( this, SIGNAL(textChanged(const QString&)), this, SLOT(watchText(const QString&)) );
}

vspFieldEdit::~vspFieldEdit (  )
{
}


void
vspFieldEdit::watchText( const QString& s )
{
  if ( hasAcceptableInput( ) )
    emit valueReady ( s );
}


vspFileInput::vspFileInput( QWidget *parent, const QStringList& f )
  : QWidget( parent ), 
    ui( new Ui::FileInput )
{
   ui->setupUi ( this );

   setFilters( f );

   connect ( ui->leFileName, SIGNAL(textChanged(const QString&)), this, SLOT(updateText(const QString&)) );
}

vspFileInput::~vspFileInput(  )
{
  delete ui;
}

void
vspFileInput::updateText( const QString& txt )
{
  setFilename( txt );
  emit textChanged( txt );
}

void
vspFileInput::onBrowse()
{
   QString startPath = vspApp->lastDir();
   QString filename = QFileDialog::getOpenFileName ( this, "Open File", startPath, d_filters.join(";;") );

   setFileName ( filename );

   if ( filename.isEmpty() )
     return;

   QFileInfo fi ( filename );
   if ( fi.exists() )
     vspApp->setLastDir ( fi.canonicalPath() );   
}


void
vspFileInput::setFilters ( const QStringList& f )
{
  d_filters = f;
}

void 
vspFileInput::setFilename ( const QString& s )
{ 
  d_filename = s; 
  ui->leFileName->setText ( s ); 
}


void
vspFileInput::setFileName( const QString& fn )
{
  if ( useRelative() && ! baseDirectory().isEmpty() )
    {
      QDir dir(d_baseDirectory);
      ui->leFileName->setText( dir.relativeFilePath(fn) );
    }
  else
    ui->leFileName->setText( fn );
}


vspDirectoryInput::vspDirectoryInput( QWidget *parent )
  : QWidget( parent ), 
    ui ( new Ui::DirectoryInput )
{
   ui->setupUi ( this );

   connect ( ui->leDirectoryName, SIGNAL(textChanged(const QString&)), this, SLOT(updateText(const QString&)) );
}

vspDirectoryInput::~vspDirectoryInput (  )
{
  delete ui;
}

void
vspDirectoryInput::updateText( const QString& txt )
{
  setDirectoryName ( txt );
  emit textChanged ( txt );
}

void
vspDirectoryInput::onBrowse()
{
   setDirectoryName ( QFileDialog::getExistingDirectory ( this, "Select Directory", QDir::currentPath() ) );
}


void
vspDirectoryInput::setDirectoryName( const QString& dn )
{
  ui->leDirectoryName->setText( dn );
}

