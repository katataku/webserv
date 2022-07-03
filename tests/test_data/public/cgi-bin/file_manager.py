#!/usr/bin/python3
import os    # 既存ファイルの有無を確認するため
import sys    # exit()するため
import cgi

upload_dir = "/var/www/localhost/files"

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

def print_debug():
	print("REQUEST_METHOD: ", os.environ['REQUEST_METHOD'], "<br>")
	print("PATH_INFO: ", os.environ['PATH_INFO'], "<br>")

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

def handle_post_method():
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
	print(" <p>you can see uploaded files <a href='/files/'>here</a></p>")

def handle_get_method():
	filename = os.environ['PATH_INFO']
	try:
		path = os.path.join(upload_dir,os.path.basename(filename))
		fin = open(path,mode='r')
		print(fin.read())
	except Exception as ee:
		print("*** error *** handle_get_method ***<br />")
		print(filename + "<br />")
		print(str (ee))
		print("<br />")

def handle_delete_method():
	filename = os.environ['PATH_INFO']
	try:
		path = os.path.join(upload_dir,os.path.basename(filename))
		os.remove(path)
		print("Success file deletion: " + filename)
	except Exception as ee:
		print("*** error *** handle_delete_method ***<br />")
		print(filename + "<br />")
		print(str (ee))
		print("<br />")


print_header()
#print_debug()

if os.environ['REQUEST_METHOD'] == 'GET':
	handle_get_method()
if os.environ['REQUEST_METHOD'] == 'POST':
	handle_post_method()
if os.environ['REQUEST_METHOD'] == 'DELETE':
	handle_delete_method()

print_footer()
