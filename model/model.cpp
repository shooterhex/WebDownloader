#include"model.h"
using namespace  std;
Model::Model()
{
    //给数据成员分配空间
    _url=std::make_shared<std::string>();
    _dir=std::make_shared<std::string>();
    _htmltxt=std::make_shared<std::string>();

};

std::shared_ptr<std::string> Model::get_Url() {
    return this->_url;
}

std::shared_ptr<std::string> Model::get_Dir() {
    return this->_dir;
}

std::shared_ptr<std::string> Model::get_Htmltxt() {
    return this->_htmltxt;
}

bool Model::setUrl(const std::string& str) {
    *_url=str;
    return true;
}

bool Model::setDir(const std::string& str) {
    *_dir=str;
    return true;
}

bool Model::setType(const int &type) {
    this->type=type;
    return true;
}
//下面实3个空函数，需要填充实际运行代码
//此处只是为了便于编译通过
//【by：田文杰】如果后续需要修改返回值类型，修改后的类型必须可以转换成bool
bool Model::downLoad()
{
    //无法保证url和dir合法，先进行简单检查
    if(_url->empty())
    {
        QMessageBox::information(nullptr,"Error","The URL is invalid!");
        return false;
    }
    else if(_dir->empty())
    {
        QMessageBox::information(nullptr,"Error","The target directory is invalid!");
        return false;
    }
    QMessageBox::information(nullptr,"download","begin");//测试用

    CURL *curl_handle;
    CURLcode res;
    MemoryStruct mem;
    string message;
    ofstream out;

    curl_global_init(CURL_GLOBAL_ALL);

    curl_handle = curl_easy_init();

    if(curl_handle)
    {
        curl_easy_setopt(curl_handle, CURLOPT_URL, _url->c_str());
        curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&mem);
        res = curl_easy_perform(curl_handle);

        if(res == CURLE_OK)
        {
            switch(type) {
            case TYPE_HTML:
                out.open(_dir->c_str());
                out << mem.memory;
                out.close();
                message = to_string(mem.size);
                message += " bytes retrieved.\n";
                break;
            case TYPE_TEXT:
                out.open(_dir->c_str());
                out << txt_proc(mem);
                out.close();
                message = to_string(mem.size);
                message += " bytes retrieved.\n";
                break;
            case TYPE_PHOTO:
                if(image_proc(mem))
                    message = "Images are downloaded.\n";
                else
                    message = "Save images failed, check your path is whether valid.\n";
                break;
            }
        }
        else
        {
            message = curl_easy_strerror(res);
            message = "curl_easy_perform() failed: " + message + ".\n";
        }
    }
    else
        message = "curl handle cannot be initialized!\n";
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();

    //最好另开线程进行下载，否则下载时间太长时，整个程序会因等待下载而失去响应
    //不过第一轮迭代不要求这一点，之后再添加
    //此处不进行实际保存，存到变量_htmltxt中即可

    QMessageBox::information(nullptr,"Download Status", message.c_str());
    if(res == CURLE_OK)
        return true;
    else
        return false;
};


size_t Model::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  MemoryStruct* memp = (MemoryStruct*)userp;

  memp->memory.resize(memp->size + realsize + 1);
  memcpy(&memp->memory[memp->size], contents, realsize);
  memp->size += realsize;
  memp->memory[memp->size] = 0;

  return realsize;
}

string Model::txt_proc(MemoryStruct& mem)
{
    vector<tag_node>tags;
    tag_node curnode;
    string res;

    curnode.txt_start = mem.memory.find("title>");
    curnode.txt_start += 6;
    mem.memory = mem.memory.substr(curnode.txt_start);
    curnode.txt_end = mem.memory.find("</");
    res += mem.memory.substr(0, curnode.txt_end) + "\n\n";
    //Min Heap + Switch + Enum
    //Insert first
    curnode.txt_end = mem.memory.find("</a>"); // aherf
    if(curnode.txt_end != -1)
    {
        curnode.tag_type = aherf;
        tags.push_back(curnode);
    }
    curnode.txt_end = mem.memory.find("title=\""); // title_eq !!This is the only forward find
    if(curnode.txt_end != -1)
    {
        curnode.tag_type = title_eq;
        tags.push_back(curnode);
    }
    curnode.txt_end = mem.memory.find("</span>"); // span
    if(curnode.txt_end != -1)
    {
        curnode.tag_type = span;
        tags.push_back(curnode);
    }
    curnode.txt_end = mem.memory.find("</p>"); // p_syntax
    if(curnode.txt_end != -1)
    {
        curnode.tag_type = p_syntax;
        tags.push_back(curnode);
    }
    curnode.txt_end = mem.memory.find("<br />"); // br_syntax
    if(curnode.txt_end != -1)
    {
        curnode.tag_type = br_syntax;
        tags.push_back(curnode);
    }
    curnode.txt_end = mem.memory.find("</h1>"); // h1
    if(curnode.txt_end != -1)
    {
        curnode.tag_type = h1;
        tags.push_back(curnode);
    }
    curnode.txt_end = mem.memory.find("</h2>"); // h2
    if(curnode.txt_end != -1)
    {
        curnode.tag_type = h2;
        tags.push_back(curnode);
    }
    curnode.txt_end = mem.memory.find("</h3>"); // h3
    if(curnode.txt_end != -1)
    {
        curnode.tag_type = h3;
        tags.push_back(curnode);
    }
    curnode.txt_end = mem.memory.find("</h4>"); // h4
    if(curnode.txt_end != -1)
    {
        curnode.tag_type = h4;
        tags.push_back(curnode);
    }
    curnode.txt_end = mem.memory.find("</h5>"); // h5
    if(curnode.txt_end != -1)
    {
        curnode.tag_type = h5;
        tags.push_back(curnode);
    }
    curnode.txt_end = mem.memory.find("</h6>"); // h6
    if(curnode.txt_end != -1)
    {
        curnode.tag_type = h6;
        tags.push_back(curnode);
    }
    make_heap(tags.begin(), tags.end(), ::greater{});
    while(!tags.empty())
    {
        pop_heap(tags.begin(), tags.end(), ::greater{});
        curnode = tags.back();
        tags.pop_back();
        
        switch (curnode.tag_type) {
        case aherf:
            curnode.txt_start = mem.memory.rfind('>', curnode.txt_end) + 1;
            if(curnode.txt_start == curnode.txt_end || mem.memory[curnode.txt_start] == '\n')
            {
                mem.memory = mem.memory.substr(curnode.txt_end + 4);
                for(auto& i : tags)
                    i.txt_end -= curnode.txt_end + 4;

                curnode.txt_end = mem.memory.find("</a>"); // aherf
                if(curnode.txt_end != -1)
                {
                    curnode.tag_type = aherf;
                    tags.push_back(curnode);
                }
                push_heap(tags.begin(), tags.end(), ::greater{});
                continue;
            }
            res += mem.memory.substr(curnode.txt_start, curnode.txt_end - curnode.txt_start) + "\n";
            mem.memory = mem.memory.substr(curnode.txt_end + 4);
            for(auto& i : tags)
                i.txt_end -= curnode.txt_end + 4;

            curnode.txt_end = mem.memory.find("</a>"); // aherf
            if(curnode.txt_end != -1)
            {
                curnode.tag_type = aherf;
                tags.push_back(curnode);
            }
            push_heap(tags.begin(), tags.end(), ::greater{});
            break;
        case title_eq:
            curnode.txt_start = curnode.txt_end + 7;
            curnode.txt_end = mem.memory.find('\"', curnode.txt_start);
            if(curnode.txt_start == curnode.txt_end || mem.memory[curnode.txt_start] == '\n')
            {
                mem.memory = mem.memory.substr(curnode.txt_end + 1);
                for(auto& i : tags)
                    i.txt_end -= curnode.txt_end + 1;

                curnode.txt_end = mem.memory.find("title=\""); // title_eq !!This is the only forward find
                if(curnode.txt_end != -1)
                {
                    curnode.tag_type = title_eq;
                    tags.push_back(curnode);
                }
                push_heap(tags.begin(), tags.end(), ::greater{});
                continue;
            }
            res += mem.memory.substr(curnode.txt_start, curnode.txt_end - curnode.txt_start) + "\n";
            mem.memory = mem.memory.substr(curnode.txt_end + 1);
            for(auto& i : tags)
                i.txt_end -= curnode.txt_end + 1;

            curnode.txt_end = mem.memory.find("title=\""); // title_eq !!This is the only forward find
            if(curnode.txt_end != -1)
            {
                curnode.tag_type = title_eq;
                tags.push_back(curnode);
            }
            push_heap(tags.begin(), tags.end(), ::greater{});
            break;
        case span:
            curnode.txt_start = mem.memory.rfind('>', curnode.txt_end) + 1;
            if(curnode.txt_start == curnode.txt_end || mem.memory[curnode.txt_start] == '\n')
            {
                mem.memory = mem.memory.substr(curnode.txt_end + 7);
                for(auto& i : tags)
                    i.txt_end -= curnode.txt_end + 7;

                curnode.txt_end = mem.memory.find("</span>"); // span
                if(curnode.txt_end != -1)
                {
                    curnode.tag_type = span;
                    tags.push_back(curnode);
                }
                push_heap(tags.begin(), tags.end(), ::greater{});
                continue;
            }
            res += mem.memory.substr(curnode.txt_start, curnode.txt_end - curnode.txt_start) + "\n";
            mem.memory = mem.memory.substr(curnode.txt_end + 7);
            for(auto& i : tags)
                i.txt_end -= curnode.txt_end + 7;
            
            curnode.txt_end = mem.memory.find("</span>"); // span
            if(curnode.txt_end != -1)
            {
                curnode.tag_type = span;
                tags.push_back(curnode);
            }
            push_heap(tags.begin(), tags.end(), ::greater{});
            break;
        case p_syntax:
            curnode.txt_start = mem.memory.rfind('>', curnode.txt_end) + 1;
            if(curnode.txt_start == curnode.txt_end || mem.memory[curnode.txt_start] == '\n')
            {
                mem.memory = mem.memory.substr(curnode.txt_end + 4);
                for(auto& i : tags)
                    i.txt_end -= curnode.txt_end + 4;

                curnode.txt_end = mem.memory.find("</p>"); // p_syntax
                if(curnode.txt_end != -1)
                {
                    curnode.tag_type = p_syntax;
                    tags.push_back(curnode);
                }
                push_heap(tags.begin(), tags.end(), ::greater{});
                continue;
            }
            res += mem.memory.substr(curnode.txt_start, curnode.txt_end - curnode.txt_start) + "\n";
            mem.memory = mem.memory.substr(curnode.txt_end + 4);
            for(auto& i : tags)
                i.txt_end -= curnode.txt_end + 4;

            curnode.txt_end = mem.memory.find("</p>"); // p_syntax
            if(curnode.txt_end != -1)
            {
                curnode.tag_type = p_syntax;
                tags.push_back(curnode);
            }
            push_heap(tags.begin(), tags.end(), ::greater{});
            break;
        case br_syntax:
            curnode.txt_start = mem.memory.rfind('>', curnode.txt_end) + 1;
            if(curnode.txt_start == curnode.txt_end || mem.memory[curnode.txt_start] == '\n')
            {
                mem.memory = mem.memory.substr(curnode.txt_end + 6);
                for(auto& i : tags)
                    i.txt_end -= curnode.txt_end + 6;
                
                curnode.txt_end = mem.memory.find("<br />"); // br_syntax
                if(curnode.txt_end != -1)
                {
                    curnode.tag_type = br_syntax;
                    tags.push_back(curnode);
                }
                push_heap(tags.begin(), tags.end(), ::greater{});
                continue;
            }
            res += mem.memory.substr(curnode.txt_start, curnode.txt_end - curnode.txt_start) + "\n";
            mem.memory = mem.memory.substr(curnode.txt_end + 6);
            for(auto& i : tags)
                i.txt_end -= curnode.txt_end + 6;

            curnode.txt_end = mem.memory.find("<br />"); // br_syntax
            if(curnode.txt_end != -1)
            {
                curnode.tag_type = br_syntax;
                tags.push_back(curnode);
            }
            push_heap(tags.begin(), tags.end(), ::greater{});
            break;
        case h1:
            curnode.txt_start = mem.memory.rfind('>', curnode.txt_end) + 1;
            if(curnode.txt_start == curnode.txt_end || mem.memory[curnode.txt_start] == '\n')
            {
                mem.memory = mem.memory.substr(curnode.txt_end + 5);
                for(auto& i : tags)
                    i.txt_end -= curnode.txt_end + 5;
                curnode.txt_end = mem.memory.find("</h1>"); // h1
                if(curnode.txt_end != -1)
                {
                    curnode.tag_type = h1;
                    tags.push_back(curnode);
                }
                push_heap(tags.begin(), tags.end(), ::greater{});
                continue;
            }
            res += mem.memory.substr(curnode.txt_start, curnode.txt_end - curnode.txt_start) + "\n";
            mem.memory = mem.memory.substr(curnode.txt_end + 5);
            for(auto& i : tags)
                i.txt_end -= curnode.txt_end + 5;

            curnode.txt_end = mem.memory.find("</h1>"); // h1
            if(curnode.txt_end != -1)
            {
                curnode.tag_type = h1;
                tags.push_back(curnode);
            }
            push_heap(tags.begin(), tags.end(), ::greater{});
            break;
        case h2:
            curnode.txt_start = mem.memory.rfind('>', curnode.txt_end) + 1;
            if(curnode.txt_start == curnode.txt_end || mem.memory[curnode.txt_start] == '\n')
            {
                mem.memory = mem.memory.substr(curnode.txt_end + 5);
                for(auto& i : tags)
                    i.txt_end -= curnode.txt_end + 5;
                
                curnode.txt_end = mem.memory.find("</h2>"); // h2
                if(curnode.txt_end != -1)
                {
                    curnode.tag_type = h2;
                    tags.push_back(curnode);
                }
                push_heap(tags.begin(), tags.end(), ::greater{});
                continue;
            }
            res += mem.memory.substr(curnode.txt_start, curnode.txt_end - curnode.txt_start) + "\n";
            mem.memory = mem.memory.substr(curnode.txt_end + 5);
            for(auto& i : tags)
                i.txt_end -= curnode.txt_end + 5;

            curnode.txt_end = mem.memory.find("</h2>"); // h2
            if(curnode.txt_end != -1)
            {
                curnode.tag_type = h2;
                tags.push_back(curnode);
            }
            push_heap(tags.begin(), tags.end(), ::greater{});
            break;
        case h3:
            curnode.txt_start = mem.memory.rfind('>', curnode.txt_end) + 1;
            if(curnode.txt_start == curnode.txt_end || mem.memory[curnode.txt_start] == '\n')
            {
                mem.memory = mem.memory.substr(curnode.txt_end + 5);
                for(auto& i : tags)
                    i.txt_end -= curnode.txt_end + 5;
                
                curnode.txt_end = mem.memory.find("</h3>"); // h3
                if(curnode.txt_end != -1)
                {
                    curnode.tag_type = h3;
                    tags.push_back(curnode);
                }
                push_heap(tags.begin(), tags.end(), ::greater{});
                continue;
            }
            res += mem.memory.substr(curnode.txt_start, curnode.txt_end - curnode.txt_start) + "\n";
            mem.memory = mem.memory.substr(curnode.txt_end + 5);
            for(auto& i : tags)
                i.txt_end -= curnode.txt_end + 5;
            
            curnode.txt_end = mem.memory.find("</h3>"); // h3
            if(curnode.txt_end != -1)
            {
                curnode.tag_type = h3;
                tags.push_back(curnode);
            }
            push_heap(tags.begin(), tags.end(), ::greater{});
            break;
        case h4:
            curnode.txt_start = mem.memory.rfind('>', curnode.txt_end) + 1;
            if(curnode.txt_start == curnode.txt_end || mem.memory[curnode.txt_start] == '\n')
            {
                mem.memory = mem.memory.substr(curnode.txt_end + 5);
                for(auto& i : tags)
                    i.txt_end -= curnode.txt_end + 5;

                curnode.txt_end = mem.memory.find("</h4>"); // h4
                if(curnode.txt_end != -1)
                {
                    curnode.tag_type = h4;
                    tags.push_back(curnode);
                }
                push_heap(tags.begin(), tags.end(), ::greater{});
                continue;
            }
            res += mem.memory.substr(curnode.txt_start, curnode.txt_end - curnode.txt_start) + "\n";
            mem.memory = mem.memory.substr(curnode.txt_end + 5);
            for(auto& i : tags)
                i.txt_end -= curnode.txt_end + 5;

            curnode.txt_end = mem.memory.find("</h4>"); // h4
            if(curnode.txt_end != -1)
            {
                curnode.tag_type = h4;
                tags.push_back(curnode);
            }
            push_heap(tags.begin(), tags.end(), ::greater{});
            break;
        case h5:
            curnode.txt_start = mem.memory.rfind('>', curnode.txt_end) + 1;
            if(curnode.txt_start == curnode.txt_end || mem.memory[curnode.txt_start] == '\n')
            {
                mem.memory = mem.memory.substr(curnode.txt_end + 5);
                for(auto& i : tags)
                    i.txt_end -= curnode.txt_end + 5;

                curnode.txt_end = mem.memory.find("</h5>"); // h5
                if(curnode.txt_end != -1)
                {
                    curnode.tag_type = h5;
                    tags.push_back(curnode);
                }
                push_heap(tags.begin(), tags.end(), ::greater{});
                continue;
            }
            res += mem.memory.substr(curnode.txt_start, curnode.txt_end - curnode.txt_start) + "\n";
            mem.memory = mem.memory.substr(curnode.txt_end + 5);
            for(auto& i : tags)
                i.txt_end -= curnode.txt_end + 5;

            curnode.txt_end = mem.memory.find("</h5>"); // h5
            if(curnode.txt_end != -1)
            {
                curnode.tag_type = h5;
                tags.push_back(curnode);
            }
            push_heap(tags.begin(), tags.end(), ::greater{});
            break;
        case h6:
            curnode.txt_start = mem.memory.rfind('>', curnode.txt_end) + 1;
            if(curnode.txt_start == curnode.txt_end || mem.memory[curnode.txt_start] == '\n')
            {
                mem.memory = mem.memory.substr(curnode.txt_end + 5);
                for(auto& i : tags)
                    i.txt_end -= curnode.txt_end + 5;
                
                curnode.txt_end = mem.memory.find("</h6>"); // h6
                if(curnode.txt_end != -1)
                {
                    curnode.tag_type = h6;
                    tags.push_back(curnode);
                }
                push_heap(tags.begin(), tags.end(), ::greater{});
                continue;
            }
            res += mem.memory.substr(curnode.txt_start, curnode.txt_end - curnode.txt_start) + "\n";
            mem.memory = mem.memory.substr(curnode.txt_end + 5);
            for(auto& i : tags)
                i.txt_end -= curnode.txt_end + 5;
                
            curnode.txt_end = mem.memory.find("</h6>"); // h6
            if(curnode.txt_end != -1)
            {
                curnode.tag_type = h6;
                tags.push_back(curnode);
            }
            push_heap(tags.begin(), tags.end(), ::greater{});
            break;
        }
    }
    return res;
}

bool Model::image_proc(MemoryStruct& mem)
{
    //find jpg, png, bmp, gif, jpeg, svg
    int start, end, count;
    string img_url, filetype[6];
    string path, filesearch;
    filetype[0]=".jpg";
    filetype[1]=".png";
    filetype[2]=".bmp";
    filetype[3]=".gif";
    filetype[4]=".jpeg";
    filetype[5]=".svg";

    for(auto& i : filetype)
    {
        count = 1;
        filesearch = mem.memory;
        while(1)
        {
            end = filesearch.find(i);
            if(end == -1)
                break;
            start = filesearch.rfind("//", end) + 2;
            img_url = filesearch.substr(start, end - start) + i;
            filesearch = filesearch.substr(end + i.size());
            if(!image_download(img_url, *_dir + "pic" + to_string(count) + i))
                return false;
            count++;
        }
    }
    return true;
}

bool Model::image_download(string& img_url, string path)
{
    //curl thing
    FILE* output_img;
    CURL* image; 
    CURLcode imgresult;

    image = curl_easy_init(); 
    if(image)
    {
        // Open file 
        output_img = fopen(path.c_str(), "wb"); 
        if(output_img == NULL)
        {
            QMessageBox::information(nullptr,"Error", "File opened failed."); 
            return false;
        }
        curl_easy_setopt(image, CURLOPT_URL, img_url.c_str()); 
        curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, NULL); 
        curl_easy_setopt(image, CURLOPT_WRITEDATA, output_img); 

        imgresult = curl_easy_perform(image); 
        if(imgresult)
            QMessageBox::information(nullptr,"Error", "Image download failed.");
    }
    curl_easy_cleanup(image);
    fclose(output_img);
    return true;
}
