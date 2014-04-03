/*
	Copyright (C) 2009-2013 jakago

	This file is part of CaptureStream, the flv downloader for NHK radio
	language courses.

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "messagewindow.h"
#include "ui_messagewindow.h"
#include "mainwindow.h"
#include "utility.h"

#include <QtGui>
#include <QTextCursor>
#include <QSettings>
#include <QVariant>

namespace {
	const QString SETTING_GROUP( "MessageWindow" );
	const QString SETTING_GEOMETRY( "geometry" );
	const int DEFAULT_WIDTH = 448;
	const int DEFAULT_HEIGHT = 300;
}

MessageWindow::MessageWindow(QWidget *parent) :
		QWidget(parent, Qt::CustomizeWindowHint|Qt::WindowTitleHint|Qt::WindowCloseButtonHint)/*, ui(new Ui::MessageWindow)*/ {
	//ui->setupUi(this);
	setupGui();
	settings( false );
}

MessageWindow::~MessageWindow() {
	settings( true );
	//delete ui;
}

void MessageWindow::changeEvent( QEvent *e ) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
		//ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MessageWindow::setupGui() {
	textEdit = new QPlainTextEdit;
	textEdit->setReadOnly( true );
	textEdit->setWordWrapMode( QTextOption::WrapAnywhere );

	clearTextButton = new QPushButton( QString::fromUtf8( "クリア" ) );
	connect( clearTextButton, SIGNAL(clicked()), this, SLOT(clearText()) );

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(textEdit);
	mainLayout->addWidget(clearTextButton);

	setLayout(mainLayout);
}

QString MessageWindow::text() {
	return textEdit->toPlainText();
}

// 改行あり
void MessageWindow::appendParagraph( const QString& text ) {
	if ( !Utility::nogui() ) {
		show();
		textEdit->appendPlainText( text );
	}
}

// 改行なし
void MessageWindow::append( const QString& text ) {
	if ( !Utility::nogui() ) {
		show();
		//textEdit->setPlainText( textEdit->toPlainText().append( text ) );	// これはとても遅い
		QTextCursor cursor = textEdit->textCursor();
		cursor.movePosition( QTextCursor::End );
		textEdit->setTextCursor( cursor );
		textEdit->insertPlainText( text );
	}
}

void MessageWindow::clearText() {
	textEdit->clear();
}

void MessageWindow::settings( bool write ) {
#if defined( Q_WS_MAC ) || defined( Q_WS_WIN )
	QSettings settings( Utility::applicationBundlePath() + INI_FILE, QSettings::IniFormat );
	settings.beginGroup( SETTING_GROUP );

	if ( !write ) {
		QVariant saved = settings.value( SETTING_GEOMETRY );
		if ( saved.type() != QVariant::Invalid )
			restoreGeometry( saved.toByteArray() );
		else
			resize( DEFAULT_WIDTH, DEFAULT_HEIGHT );
	} else {
		settings.setValue( SETTING_GEOMETRY, saveGeometry() );
	}

	settings.endGroup();
#else
	Q_UNUSED( write )
	resize( DEFAULT_WIDTH, DEFAULT_HEIGHT );
#endif
}
