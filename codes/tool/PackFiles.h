#ifndef _TNG_TOOL_PACK_H
#define _TNG_TOOL_PACK_H
#include <QFile>
#include <QByteArray>

namespace tng
{
	__inline void Zip(QString filename, QString zip_filename)
	{
		QFile infile(filename);
		QFile outfile(zip_filename);
		infile.open(QIODevice::ReadOnly);
		outfile.open(QIODevice::WriteOnly);
		QByteArray uncompressed_data = infile.readAll();
		QByteArray compressed_data = qCompress(uncompressed_data, 9);
		outfile.write(compressed_data);
		infile.close();
		outfile.close();
	}

	__inline void UnZip(QString zip_filename, QString filename)
	{
		QFile infile(zip_filename);
		QFile outfile(filename);
		infile.open(QIODevice::ReadOnly);
		outfile.open(QIODevice::WriteOnly);
		QByteArray uncompressed_data = infile.readAll();
		QByteArray compressed_data = qUncompress(uncompressed_data);
		outfile.write(compressed_data);
		infile.close();
		outfile.close();
	}
}
#endif