from ipykernel.kernelbase import Kernel

class NielsKernel(Kernel):
    implementation = 'Niels'
    implementation_version = '0.1'
    language = 'niels'
    language_version = '0.1'
    language_info = {'mimetype': 'text/niels'}
    banner = "Niels - Numerically Intensive Expression Language for Science"

    def do_execute(self, code, silent, store_history=True, user_expressions=None,
                   allow_stdin=False):
        if not silent:
            stream_content = {'name': 'stdout', 'text': code}
            self.send_response(self.iopub_socket, 'stream', stream_content)

        return {
            'status': 'ok',
            'execution_count': self.execution_count,
            'payload': [],
            'user_expressions': {},
       }

if __name__ == '__main__':
    from ipykernel.kernelapp import IPKernelApp
    IPKernelApp.launch_instance(kernel_class=NielsKernel)
