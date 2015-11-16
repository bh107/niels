from IPython.kernel.zmq.kernelapp import IPKernelApp
from .kernel import NielsKernel
IPKernelApp.launch_instance(kernel_class=NielsKernel)
