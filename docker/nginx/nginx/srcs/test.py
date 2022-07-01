#!/usr/bin/python3
import os    # 既存ファイルの有無を確認するため
import sys    # exit()するため
import cgi

def print_header():
	print('Content-Type: text/html')
	print('')
	print('<!DOCTYPE html>')
	print('')
	print('<html lang="ja">')
	print('')
	print('<head>')
	print('  <meta charset="UTF-8">')
	print('  <meta name="description" content="file upload">')
	print('  <meta name="keywords" content="upload">')
	print('  <title>File upload</title>')
	print('</head>')
	print('')
	print('<body>')

def print_footer():
	print('</body>')
	print('')
	print('</html>')
	sys.exit(0)

def single_upload_file_proc(upload_dir,item):
	filename = item.filename
	try:
		path = os.path.join(upload_dir,os.path.basename(filename))
		chunk = item.file.read()
		if chunk:
			fout = open(path,mode='wb')
			fout.write(chunk)
			fout.close()
			os.chmod(path, 0o666)
			print("<p>file upload success</p>")
	except Exception as ee:
		print("*** error *** single_upload_file_proc ***<br />")
		print(filename + "<br />")
		print(str (ee))
		print("<br />")

def multi_uploaded_file_proc(upload_dir,fileitem):
    for item in fileitem:
        single_upload_file_proc(upload_dir,item)



upload_dir = "/var/www/localhost/files"

print_header()

print(os.environ['REQUEST_METHOD'])
form = cgi.FieldStorage()
print(form)

if "upload_file" in form:
	fileitem = form["upload_file"]
	if (isinstance(fileitem,list)):
		multi_uploaded_file_proc(upload_dir,fileitem)
	else:
		single_upload_file_proc(upload_dir,fileitem)
else:
	print("*** Select files ***<br />")


print_footer()