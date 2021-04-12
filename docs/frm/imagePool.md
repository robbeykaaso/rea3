# Abstract
a singleton for caching images by their paths for different threads  

# API
* **void cacheImage(const QString& aPath, const QImage& aImage)**  
cache a image by its path into a queue  
</br>

* **QImage readCache(const QString& aPath)**  
read the cached image. if it doesn't exisit, it will try to read it from the file system  
</br>