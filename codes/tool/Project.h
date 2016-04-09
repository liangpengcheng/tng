#ifndef _TNG_PROJECT_H_
#define _TNG_PROJECT_H_
#include <QApplication>
#include <QtGui>
#include <QtUiTools/QtUiTools>
#include <QMainWindow>
#include <QtWidgets/QWidget>
#include "graphics/graphics.h"
#include "core/filesystem.h"
#include "core/path.h"
namespace tng
{
	class ProjectMain :public QMainWindow
	{
		Q_OBJECT
	public:
		void Init();
		void LoadProject(const string& path);
		void Export(const string& path);
	 
	private slots :
		//actions
		void OnLoad();
		void OnExport();
		//
	protected:
		QWidget* main_frame_;
		string runing_path_;
	};
}

#endif