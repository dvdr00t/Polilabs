package conversion;

import static io.grpc.MethodDescriptor.generateFullMethodName;
import static io.grpc.stub.ClientCalls.asyncBidiStreamingCall;
import static io.grpc.stub.ClientCalls.asyncClientStreamingCall;
import static io.grpc.stub.ClientCalls.asyncServerStreamingCall;
import static io.grpc.stub.ClientCalls.asyncUnaryCall;
import static io.grpc.stub.ClientCalls.blockingServerStreamingCall;
import static io.grpc.stub.ClientCalls.blockingUnaryCall;
import static io.grpc.stub.ClientCalls.futureUnaryCall;
import static io.grpc.stub.ServerCalls.asyncBidiStreamingCall;
import static io.grpc.stub.ServerCalls.asyncClientStreamingCall;
import static io.grpc.stub.ServerCalls.asyncServerStreamingCall;
import static io.grpc.stub.ServerCalls.asyncUnaryCall;
import static io.grpc.stub.ServerCalls.asyncUnimplementedStreamingCall;
import static io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall;

/**
 * <pre>
 * The conversion service definition.
 * </pre>
 */
@javax.annotation.Generated(
    value = "by gRPC proto compiler (version 1.31.1)",
    comments = "Source: conversion.proto")
public final class ConverterGrpc {

  private ConverterGrpc() {}

  public static final String SERVICE_NAME = "conversion.Converter";

  // Static method descriptors that strictly reflect the proto.
  private static volatile io.grpc.MethodDescriptor<conversion.ConversionRequest,
      conversion.ConversionReply> getFileConvertMethod;

  @io.grpc.stub.annotations.RpcMethod(
      fullMethodName = SERVICE_NAME + '/' + "fileConvert",
      requestType = conversion.ConversionRequest.class,
      responseType = conversion.ConversionReply.class,
      methodType = io.grpc.MethodDescriptor.MethodType.BIDI_STREAMING)
  public static io.grpc.MethodDescriptor<conversion.ConversionRequest,
      conversion.ConversionReply> getFileConvertMethod() {
    io.grpc.MethodDescriptor<conversion.ConversionRequest, conversion.ConversionReply> getFileConvertMethod;
    if ((getFileConvertMethod = ConverterGrpc.getFileConvertMethod) == null) {
      synchronized (ConverterGrpc.class) {
        if ((getFileConvertMethod = ConverterGrpc.getFileConvertMethod) == null) {
          ConverterGrpc.getFileConvertMethod = getFileConvertMethod =
              io.grpc.MethodDescriptor.<conversion.ConversionRequest, conversion.ConversionReply>newBuilder()
              .setType(io.grpc.MethodDescriptor.MethodType.BIDI_STREAMING)
              .setFullMethodName(generateFullMethodName(SERVICE_NAME, "fileConvert"))
              .setSampledToLocalTracing(true)
              .setRequestMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  conversion.ConversionRequest.getDefaultInstance()))
              .setResponseMarshaller(io.grpc.protobuf.ProtoUtils.marshaller(
                  conversion.ConversionReply.getDefaultInstance()))
              .setSchemaDescriptor(new ConverterMethodDescriptorSupplier("fileConvert"))
              .build();
        }
      }
    }
    return getFileConvertMethod;
  }

  /**
   * Creates a new async stub that supports all call types for the service
   */
  public static ConverterStub newStub(io.grpc.Channel channel) {
    io.grpc.stub.AbstractStub.StubFactory<ConverterStub> factory =
      new io.grpc.stub.AbstractStub.StubFactory<ConverterStub>() {
        @java.lang.Override
        public ConverterStub newStub(io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
          return new ConverterStub(channel, callOptions);
        }
      };
    return ConverterStub.newStub(factory, channel);
  }

  /**
   * Creates a new blocking-style stub that supports unary and streaming output calls on the service
   */
  public static ConverterBlockingStub newBlockingStub(
      io.grpc.Channel channel) {
    io.grpc.stub.AbstractStub.StubFactory<ConverterBlockingStub> factory =
      new io.grpc.stub.AbstractStub.StubFactory<ConverterBlockingStub>() {
        @java.lang.Override
        public ConverterBlockingStub newStub(io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
          return new ConverterBlockingStub(channel, callOptions);
        }
      };
    return ConverterBlockingStub.newStub(factory, channel);
  }

  /**
   * Creates a new ListenableFuture-style stub that supports unary calls on the service
   */
  public static ConverterFutureStub newFutureStub(
      io.grpc.Channel channel) {
    io.grpc.stub.AbstractStub.StubFactory<ConverterFutureStub> factory =
      new io.grpc.stub.AbstractStub.StubFactory<ConverterFutureStub>() {
        @java.lang.Override
        public ConverterFutureStub newStub(io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
          return new ConverterFutureStub(channel, callOptions);
        }
      };
    return ConverterFutureStub.newStub(factory, channel);
  }

  /**
   * <pre>
   * The conversion service definition.
   * </pre>
   */
  public static abstract class ConverterImplBase implements io.grpc.BindableService {

    /**
     */
    public io.grpc.stub.StreamObserver<conversion.ConversionRequest> fileConvert(
        io.grpc.stub.StreamObserver<conversion.ConversionReply> responseObserver) {
      return asyncUnimplementedStreamingCall(getFileConvertMethod(), responseObserver);
    }

    @java.lang.Override public final io.grpc.ServerServiceDefinition bindService() {
      return io.grpc.ServerServiceDefinition.builder(getServiceDescriptor())
          .addMethod(
            getFileConvertMethod(),
            asyncBidiStreamingCall(
              new MethodHandlers<
                conversion.ConversionRequest,
                conversion.ConversionReply>(
                  this, METHODID_FILE_CONVERT)))
          .build();
    }
  }

  /**
   * <pre>
   * The conversion service definition.
   * </pre>
   */
  public static final class ConverterStub extends io.grpc.stub.AbstractAsyncStub<ConverterStub> {
    private ConverterStub(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected ConverterStub build(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      return new ConverterStub(channel, callOptions);
    }

    /**
     */
    public io.grpc.stub.StreamObserver<conversion.ConversionRequest> fileConvert(
        io.grpc.stub.StreamObserver<conversion.ConversionReply> responseObserver) {
      return asyncBidiStreamingCall(
          getChannel().newCall(getFileConvertMethod(), getCallOptions()), responseObserver);
    }
  }

  /**
   * <pre>
   * The conversion service definition.
   * </pre>
   */
  public static final class ConverterBlockingStub extends io.grpc.stub.AbstractBlockingStub<ConverterBlockingStub> {
    private ConverterBlockingStub(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected ConverterBlockingStub build(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      return new ConverterBlockingStub(channel, callOptions);
    }
  }

  /**
   * <pre>
   * The conversion service definition.
   * </pre>
   */
  public static final class ConverterFutureStub extends io.grpc.stub.AbstractFutureStub<ConverterFutureStub> {
    private ConverterFutureStub(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected ConverterFutureStub build(
        io.grpc.Channel channel, io.grpc.CallOptions callOptions) {
      return new ConverterFutureStub(channel, callOptions);
    }
  }

  private static final int METHODID_FILE_CONVERT = 0;

  private static final class MethodHandlers<Req, Resp> implements
      io.grpc.stub.ServerCalls.UnaryMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ServerStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ClientStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.BidiStreamingMethod<Req, Resp> {
    private final ConverterImplBase serviceImpl;
    private final int methodId;

    MethodHandlers(ConverterImplBase serviceImpl, int methodId) {
      this.serviceImpl = serviceImpl;
      this.methodId = methodId;
    }

    @java.lang.Override
    @java.lang.SuppressWarnings("unchecked")
    public void invoke(Req request, io.grpc.stub.StreamObserver<Resp> responseObserver) {
      switch (methodId) {
        default:
          throw new AssertionError();
      }
    }

    @java.lang.Override
    @java.lang.SuppressWarnings("unchecked")
    public io.grpc.stub.StreamObserver<Req> invoke(
        io.grpc.stub.StreamObserver<Resp> responseObserver) {
      switch (methodId) {
        case METHODID_FILE_CONVERT:
          return (io.grpc.stub.StreamObserver<Req>) serviceImpl.fileConvert(
              (io.grpc.stub.StreamObserver<conversion.ConversionReply>) responseObserver);
        default:
          throw new AssertionError();
      }
    }
  }

  private static abstract class ConverterBaseDescriptorSupplier
      implements io.grpc.protobuf.ProtoFileDescriptorSupplier, io.grpc.protobuf.ProtoServiceDescriptorSupplier {
    ConverterBaseDescriptorSupplier() {}

    @java.lang.Override
    public com.google.protobuf.Descriptors.FileDescriptor getFileDescriptor() {
      return conversion.ConversionProto.getDescriptor();
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.ServiceDescriptor getServiceDescriptor() {
      return getFileDescriptor().findServiceByName("Converter");
    }
  }

  private static final class ConverterFileDescriptorSupplier
      extends ConverterBaseDescriptorSupplier {
    ConverterFileDescriptorSupplier() {}
  }

  private static final class ConverterMethodDescriptorSupplier
      extends ConverterBaseDescriptorSupplier
      implements io.grpc.protobuf.ProtoMethodDescriptorSupplier {
    private final String methodName;

    ConverterMethodDescriptorSupplier(String methodName) {
      this.methodName = methodName;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.MethodDescriptor getMethodDescriptor() {
      return getServiceDescriptor().findMethodByName(methodName);
    }
  }

  private static volatile io.grpc.ServiceDescriptor serviceDescriptor;

  public static io.grpc.ServiceDescriptor getServiceDescriptor() {
    io.grpc.ServiceDescriptor result = serviceDescriptor;
    if (result == null) {
      synchronized (ConverterGrpc.class) {
        result = serviceDescriptor;
        if (result == null) {
          serviceDescriptor = result = io.grpc.ServiceDescriptor.newBuilder(SERVICE_NAME)
              .setSchemaDescriptor(new ConverterFileDescriptorSupplier())
              .addMethod(getFileConvertMethod())
              .build();
        }
      }
    }
    return result;
  }
}
