FROM quay.io/pypa/manylinux2014_x86_64
RUN yum -y install eigen3-devel boost-devel libgomp glm-devel
RUN yum -y install mlocate
RUN yum -y install gcc
RUN updatedb
RUN /opt/python/cp37-cp37m/bin/python -m pip install numpy tqdm cython pytest
RUN /opt/python/cp38-cp38/bin/python -m pip install numpy tqdm cython pytest
RUN /opt/python/cp39-cp39/bin/python -m pip install numpy tqdm cython pytest
RUN /opt/python/cp310-cp310/bin/python -m pip install numpy tqdm cython pytest
RUN /opt/python/cp311-cp311/bin/python -m pip install numpy tqdm cython pytest
