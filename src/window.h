/*
	SPDX-FileCopyrightText: 2009-2010 Graeme Gott <graeme@gottcode.org>

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef HEXALATE_WINDOW_H
#define HEXALATE_WINDOW_H

#include <QMainWindow>

class Window : public QMainWindow
{
	Q_OBJECT

public:
	explicit Window();

protected:
	void closeEvent(QCloseEvent* event) override;

private slots:
	void setLocale();
	void about();
};

#endif // HEXALATE_WINDOW_H
