#include "Project.h"
#include <QAction>
#include <QFileDialog>
#include "modelimport/port.h"
#include "core/binary_reader_writer.h"
namespace tng
{
	void ProjectMain::Init()
	{
		tng::Log* log = new tng::ConsoleLog;
		tng::Log::SetLog(log);
		string exename, exepath, exeext;
		Path::GetExePath().Split(exepath, exename, exeext);
		Path::SetWorkPath(exepath);
		//ResourceManger::GetInstance()->AddLoader(LocalFilesLoader);

		graphics::InitGraphics();
		GFXService* gfx = GFXService::CreateGraphics();
		InitModelImpPlugin();
		QUiLoader loader;

		QFile file("../EditorAsset/MainWindow.ui");
		file.open(QFile::ReadOnly);

		main_frame_ = loader.load(&file);
		file.close();

		if (main_frame_)
		{
			main_frame_->setWindowFlags(Qt::SubWindow);
		}
		setCentralWidget(main_frame_);

		QAction* loadbtn = main_frame_->findChild<QAction*>("actionLoad");
		connect(loadbtn, SIGNAL(triggered()), this, SLOT(OnLoad()));

		QAction* expbtn = main_frame_->findChild<QAction*>("actionExport");
		connect(expbtn, SIGNAL(triggered()), this, SLOT(OnExport()));


		resize(1024, 768);

	}
	void ProjectMain::OnLoad()
	{
		QFileDialog dialog;
		dialog.setFileMode(QFileDialog::DirectoryOnly);
		dialog.setOption(QFileDialog::DontUseNativeDialog, true);
		dialog.setOption(QFileDialog::DontResolveSymlinks);
		dialog.setViewMode(QFileDialog::Detail);
		int res = dialog.exec();
		if (res)
		{
			QDir directory = dialog.selectedFiles()[0];
			LoadProject(directory.absolutePath().toUtf8().data());
		}

	}

	void ProjectMain::OnExport()
	{
		QFileDialog dialog;
		dialog.setFileMode(QFileDialog::DirectoryOnly);
		dialog.setOption(QFileDialog::DontUseNativeDialog, true);
		dialog.setOption(QFileDialog::DontResolveSymlinks);
		dialog.setViewMode(QFileDialog::Detail);
		int res = dialog.exec();
		if (res)
		{
			QDir directory = dialog.selectedFiles()[0];
			Export(directory.absolutePath().toUtf8().data());
		}
	}
	void ProjectMain::LoadProject(const string& path)
	{
		runing_path_ = path;
	}
	void ProjectMain::Export(const string& path)
	{
		vector<Path> retPath;
		FileSystem::FindFiles(runing_path_, retPath, FileSystem::FFile, true);
		StreamLoader* loader = ResourceManger::GetInstance()->GetLoader(LocalFilesLoader);
		if (loader)
		{
			for each (Path var in retPath)
			{

				string fn, ext, dir;
				var.Split(dir, fn, ext);
				ToLower(ext);
				if (ext == "res")
				{
					continue;
				}
				Path relativePath = Path(dir).RelativePath(runing_path_);
				//use resource processor process every file,if no processor exist just copy to export dir
				std::istream* content = loader->Load(var);
				ResDecoder* decoder = ResourceManger::GetInstance()->GetDecoder(ext);

				if (content)
				{
					string ext_e = decoder != NULL ? decoder->ResType() : ext;
					if (ext_e == "res")
					{
						continue;
					}
					string pathexp = path + "/" + string(relativePath);
					FileSystem::CreateDirs(pathexp);
					FileOutputStream fs(pathexp + fn + "." + ext_e);
					content->seekg(0, std::ios::beg);
					fs << content->rdbuf();
					fs.flush();

				}

			}
		}

	}
}

