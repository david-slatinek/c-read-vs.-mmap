# C - read vs mmap
Read and mmap are both fundamentally important system calls, used to access bytes in files.
Read uses the standard file descriptor to access files while mmap maps files to RAM.
The purpose of this experiment was to compare performance differences with read when compared to mmap.

## Approach
1. Randomly generate file content for different file sizes.
2. Calculate file histogram and measure elapsed time - 10 times
3. Write elapsed time, average time and standard deviation to a file.

These tests were conducted on a 5.4.0-77-generic Linux Mint 20.1 system utilizing an Intel Core i7-9750H @ 2.60GHz × 6 with 9.3.0 gcc version.

# Getting started
Clone the project or download it as a zip and then unzip it.
## Conduct standard test
Run
```bash
bash script.sh -h
```
and then use the appropriate flag.

## Conduct custom test
Run
```bash
make
./main -h
```
and then use the appropriate flag.

**Note:** you can firstly use ```bash script.sh -c``` to generate test files.

# Results
Buffer sizes are in bytes, time is in milliseconds. 
\
\
File size: 512B
|                    | read(512) | read(1024) | read(4096) | read(8192) | read(16384) |  mmap  |
| :----------------: | :-------: | :--------: | :--------: | :--------: | :---------: | :----: |
|      Average       |  0.0064   |   0.0049   |   0.0053   |   0.0067   |   0.0099    | 0.0105 |
| Standard deviation |  0.0025   |   0.0022   |   0.0028   |   0.0028   |   0.0024    | 0.0048 |

Better method: read(1024), by 53.33%
<hr>

File size: 2kB
|                    | read(512) | read(1024) | read(4096) | read(8192) | read(16384) |  mmap  |
| :----------------: | :-------: | :--------: | :--------: | :--------: | :---------: | :----: |
|      Average       |  0.0096   |   0.0078   |   0.0062   |   0.0084   |   0.0115    | 0.0139 |
| Standard deviation |  0.0023   |   0.0022   |   0.0024   |   0.0022   |   0.0023    | 0.0067 |

Better method: read(4096), by 55.40%
<hr>

File size: 8kB
|                    | read(512) | read(1024) | read(4096) | read(8192) | read(16384) |  mmap  |
| :----------------: | :-------: | :--------: | :--------: | :--------: | :---------: | :----: |
|      Average       |  0.0251   |   0.0189   |   0.0135   |   0.0152   |   0.0169    | 0.0362 |
| Standard deviation |  0.0036   |   0.0033   |   0.0034   |   0.0038   |   0.0033    | 0.0209 |

Better method: read(4096), by 62.71%
<hr>

File size: 32kB
|                    | read(512) | read(1024) | read(4096) | read(8192) | read(16384) |  mmap  |
| :----------------: | :-------: | :--------: | :--------: | :--------: | :---------: | :----: |
|      Average       |  0.0859   |   0.0608   |   0.0489   |   0.0400   |   0.0410    | 0.0616 |
| Standard deviation |  0.0061   |   0.0058   |   0.0177   |   0.0068   |   0.0057    | 0.0174 |

Better method: read(8192), by 35.06%
<hr>

File size: 128kB
|                    | read(512) | read(1024) | read(4096) | read(8192) | read(16384) |  mmap  |
| :----------------: | :-------: | :--------: | :--------: | :--------: | :---------: | :----: |
|      Average       |  0.3414   |   0.2343   |   0.1532   |   0.1570   |   0.2329    | 0.1879 |
| Standard deviation |  0.0262   |   0.0136   |   0.0140   |   0.0407   |   0.0651    | 0.0307 |

Better method: read(4096), by 18.47%
<hr>

File size: 512kB
|                    | read(512) | read(1024) | read(4096) | read(8192) | read(16384) |  mmap  |
| :----------------: | :-------: | :--------: | :--------: | :--------: | :---------: | :----: |
|      Average       |  1.3204   |   0.9052   |   0.5982   |   0.5411   |   0.5155    | 0.6608 |
| Standard deviation |  0.0385   |   0.0525   |   0.0474   |   0.0433   |   0.0410    | 0.0810 |

Better method: read(16384), by 21.99%
<hr>

File size: 1MB
|                    | read(512) | read(1024) | read(4096) | read(8192) | read(16384) |  mmap  |
| :----------------: | :-------: | :--------: | :--------: | :--------: | :---------: | :----: |
|      Average       |  2.7041   |   1.8161   |   1.1783   |   1.0879   |   1.1230    | 1.9790 |
| Standard deviation |  0.1402   |   0.0729   |   0.0818   |   0.0866   |   0.0858    | 0.1878 |

Better method: read(8192), by 45.03%
<hr>

File size: 8MB
|                    | read(512) | read(1024) | read(4096) | read(8192) | read(16384) |  mmap  |
| :----------------: | :-------: | :--------: | :--------: | :--------: | :---------: | :----: |
|      Average       |  22.9465  |  15.2235   |  11.0226   |   9.5879   |   8.9895    | 8.8639 |
| Standard deviation |  2.3845   |   0.7469   |   1.7879   |   1.0304   |   0.8315    | 4.8685 |

Better method: mmap, by 1.4%
<hr>

File size: 32MB
|                    | read(512) | read(1024) | read(4096) | read(8192) | read(16384) |  mmap   |
| :----------------: | :-------: | :--------: | :--------: | :--------: | :---------: | :-----: |
|      Average       |  88.1362  |  58.6678   |  39.9459   |  35.9764   |   33.9954   | 27.9418 |
| Standard deviation |  6.7785   |   2.6128   |   2.6215   |   2.5052   |   2.8756    | 7.7543  |

Better method: mmap, by 17.81%
<hr>

File size: 256MB
|                    | read(512) | read(1024) | read(4096) | read(8192) | read(16384) |   mmap   |
| :----------------: | :-------: | :--------: | :--------: | :--------: | :---------: | :------: |
|      Average       | 682.8666  |  466.3594  |  313.6546  |  285.8377  |  271.1903   | 216.3714 |
| Standard deviation |  21.2080  |  19.3882   |  19.5440   |  18.7643   |   20.0427   | 24.3847  |

Better method: mmap, by 20.21%
<hr>

File size: 1GB
|                    | read(512) | read(1024) | read(4096) | read(8192) | read(16384) |   mmap   |
| :----------------: | :-------: | :--------: | :--------: | :--------: | :---------: | :------: |
|      Average       | 2780.2674 | 1915.0757  | 1300.4177  | 1181.8023  |  1065.5766  | 845.8249 |
| Standard deviation |  54.3265  |  67.8010   |  74.5956   |  92.7043   |   75.9196   | 95.8478  |

Better method: mmap, by 20.62%

# Conclusion
For smaller files, the **read** method is faster when compared to the **mmap** method. That changes with large files - **mmap** is faster for around 15%.
\
\
For smaller files, the most appropriate buffer size would be 4096B or 8192B, which is the same as a page size on the computer used in the experiment.
The page size can be found by running ```getconf PAGE_SIZE```.
\
\
For larger files, the most suitable buffer size would be 16kB.
